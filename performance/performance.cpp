#include "performance.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>
#include <i-ching/i-ching.h>
#include <mixer/factory.h>

logxx::Log Performance::cLog("Performance");
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
    S_LOG("Mixing");
    auto mixer = GetMixer();
    auto deck = standard_36_deck::kDeck;
    {
        static const std::size_t decksCount = 1E6;
        TimeMeasure timer;
        for (std::size_t i = 0; i != decksCount; ++i) {
            mixer->Mix(deck);
        }
        double elapsed = timer.Elapsed();
        log(logxx::info) << decksCount << " decks generated in " << elapsed
                         << "s: " << decksCount / elapsed << " decks per second"
                         << logxx::endl;
    }
}

std::vector<std::vector<Card>> Performance::PregenerateConvergableDecks() const {
    S_LOG("PregenerateConvergableDecks");
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
        log(logxx::info) << decksCount << " convergable decks generated in " << elapsed
                         << "s: " << decksCount / elapsed << " decks per second"
                         << logxx::endl;
    }
    return pregeneratedDecks;
}

void Performance::MediciGenerator() const {
    S_LOG("MediciGenerator");
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
        log(logxx::info) << pregeneratedDecks.size() << " convergable decks converged in "
                         << elapsed << "s: " << pregeneratedDecks.size() / elapsed
                         << " decks per second" << logxx::endl;
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
    S_LOG("MediciWithConditions");

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
    log(logxx::info) << totalDecks << " appropriate decks generated in " << elapsed
                     << "s: " << totalDecks / elapsed << " decks per second"
                     << logxx::endl;
}

void Performance::MediciWithConditionsAndIChing() const {
    S_LOG("MediciWithConditionsAndIChing");

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
        log(logxx::info) << balanced << " balanced decks" << logxx::endl;
    log(logxx::info) << totalDecks << " appropriate decks generated and checked in "
                     << elapsed << "s: " << totalDecks / elapsed << " decks per second"
                     << logxx::endl;
}

void Performance::IChingBalancedPercent() const {
    S_LOG("IChingBalancedPercent");
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
    log(logxx::info) << static_cast<double>(balanced) / totalDecks * 100.0
                     << "% balanced decks" << logxx::endl;
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
