#include "performance.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>
#include <i-ching/i-ching.h>
#include <mixer/factory.h>
#include <easylogging++.h>

CardSelectorConfigurator Performance::configurator;
using namespace medici;

Performance::Performance() : mixersFactory(N) {
}

void Performance::Run(MixersFactory::MixerType mixerType) {
    mixersFactory.SetMixer(mixerType);
    Mixing();
    MediciGenerator();
    MediciWithConditions();
    MediciWithConditionsAndIChing();
    IChingBalancedPercent();
}

CheckOperand::CheckOperand(DeckSelectors&& selectors)
        : deckSelectors(std::move(selectors)) {
}

bool CheckOperand::operator()(const std::vector<Card>& deck) const {
    return deckSelectors.Check(deck);
}

CheckOperand Performance::DefaultCheckOperand() {
    CheckOperand operand(DefaultSelectors());
    return operand;
}

void Performance::Mixing() const {
    auto mixer = GetMixer();
    auto deck = standard_36_deck::kDeck;
    {
        static const std::size_t decksCount = 1E6;
        TimeMeasure timer;
        for (std::size_t i = 0; i != decksCount; ++i) {
            mixer->Mix(deck);
        }
        double elapsed = timer.Elapsed();
        LOG(INFO) << decksCount << " decks generated in " << elapsed
                  << "s: " << decksCount / elapsed << " decks per second";
    }
}

std::vector<std::vector<Card>> Performance::PregenerateConvergableDecks() const {
    auto mixer = GetMixer();
    PatienceInfo info;
    std::vector<std::vector<Card>> pregeneratedDecks;
    static const std::size_t decksCount = 1E3;
    pregeneratedDecks.reserve(decksCount);
    auto deck = standard_36_deck::kDeck;
    {
        TimeMeasure timer;
        for (std::size_t i = 0; i != decksCount; ++i) {
            do {
                mixer->Mix(deck);
            } while (!TryToConverge(deck, info));
            pregeneratedDecks.push_back(deck);
        }
        double elapsed = timer.Elapsed();
        LOG(INFO) << decksCount << " convergable decks generated in " << elapsed
                  << "s: " << decksCount / elapsed << " decks per second";
    }
    return pregeneratedDecks;
}

void Performance::MediciGenerator() const {
    PatienceInfo info;

    auto pregeneratedDecks = PregenerateConvergableDecks();
    auto it = pregeneratedDecks.cbegin();
    auto end = pregeneratedDecks.cend();
    {
        TimeMeasure timer;
        for (; it != end; ++it) {
            auto& deck = *it;
            if (!TryToConverge(deck, info))
                throw std::logic_error("Convergable deck doesn't converges!");
        }
        double elapsed = timer.Elapsed();
        LOG(INFO) << pregeneratedDecks.size() << " convergable decks converged in "
                  << elapsed << "s: " << pregeneratedDecks.size() / elapsed
                  << " decks per second";
    }
}

DeckSelectors Performance::DefaultSelectors() {
    using namespace standard_36_deck;
    DeckOneSelector targetCard({Selector(Hearts, Ten)}, 19, 24);
    DeckAllSelector ownActions({SelectorAnySuit(Ace, false)}, 3, 7);
    DeckOneSelector firstCard({SelectorAnySuit(Jack)}, 0, 0);
    DeckOneSelector secondCard({SelectorAnySuit(Nine)}, 1, 1);
    DeckOneSelector thirdCard({SelectorAnySuit(Ace), SelectorAnySuit(Ten)}, 2, 2);

    DeckSelectors selectors;
    selectors.AddDeckSelector(targetCard);
    selectors.AddDeckSelector(ownActions);
    selectors.AddDeckSelector(firstCard);
    selectors.AddDeckSelector(secondCard);
    selectors.AddDeckSelector(thirdCard);
    return selectors;
}

void Performance::MediciWithConditions() const {
    auto mixer = GetMixer();
    auto deck = standard_36_deck::kDeck;
    PatienceInfo info;
    auto checker = DefaultCheckOperand();
    const std::size_t totalDecks = 100;
    TimeMeasure timer;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        do {
            mixer->Mix(deck);
        } while (!(checker(deck) && TryToConverge(deck, info)));
    }
    double elapsed = timer.Elapsed();
    LOG(INFO) << totalDecks << " appropriate decks generated in " << elapsed
              << "s: " << totalDecks / elapsed << " decks per second";
}

void Performance::MediciWithConditionsAndIChing() const {
    auto deck = standard_36_deck::kDeck;
    PatienceInfo info;
    i_ching::BalanceChecker iChingChecker;
    auto checker = DefaultCheckOperand();
    auto mixer = GetMixer();

    std::size_t totalDecks = 100;
    std::size_t balanced = 0;
    TimeMeasure timer;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        do {
            mixer->Mix(deck);
        } while (!(checker(deck) && TryToConverge(deck, info)));
        if (iChingChecker.Check(info))
            ++balanced;
    }
    double elapsed = timer.Elapsed();
    if (balanced != 0)
        LOG(INFO) << balanced << " balanced decks";
    LOG(INFO) << totalDecks << " appropriate decks generated and checked in " << elapsed
              << "s: " << totalDecks / elapsed << " decks per second";
}

void Performance::IChingBalancedPercent() const {
    auto deck = standard_36_deck::kDeck;
    PatienceInfo info;
    i_ching::BalanceChecker iChingChecker;
    auto mixer = GetMixer();

    std::size_t totalDecks = 1E5;
    std::size_t balanced = 0;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        do {
            mixer->Mix(deck);
        } while (!TryToConverge(deck, info));
        if (iChingChecker.Check(info))
            ++balanced;
    }
    LOG(INFO) << static_cast<double>(balanced) / totalDecks * 100.0 << "% balanced decks";
}

Performance::StandardGenerator Performance::GetMixer(std::uint_fast32_t seed) const {
    return mixersFactory.CreateMixer<Card>(seed);
}

CardSelector Performance::SelectorAnyRank(std::uint_fast8_t suit, bool straight) {
    configurator.Reset();
    configurator.SetSuit(suit);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}

CardSelector Performance::SelectorAnySuit(std::uint_fast8_t rank, bool straight) {
    configurator.Reset();
    configurator.SetRank(rank);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}

CardSelector Performance::Selector(std::uint_fast8_t suit, std::uint_fast8_t rank,
                                   bool straight) {
    configurator.Reset();
    configurator.SetSuit(suit);
    configurator.SetRank(rank);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}
