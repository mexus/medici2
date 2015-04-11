#include "performance.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>
#include <i-ching/i-ching.h>

logxx::Log Performance::cLog("Performance");
std::atomic_bool Performance::interrupt(false);
using namespace medici;

void Performance::Run()
{
    Mixing();
    MediciGenerator();
    MediciWithConditions();
    MediciWithConditionsAndIChing();
    IChingBalancedPercent();
}

Performance::CheckOperand::CheckOperand(DeckSelectors&& selectors) :
    deckSelectors(std::move(selectors))
{
}

bool Performance::CheckOperand::operator()(const StandardDeck& deck) const
{
    return deckSelectors.Check(deck);
}

void Performance::Mixing()
{
    S_LOG("Mixing");
    StandardMixer mixer;
    auto deck = standard_36_deck::Deck::cards;
    {
        static const std::size_t decksCount = 1E6;
        TimeMeasure timer;
        for (std::size_t i = 0; i != decksCount; ++i) {
            mixer.Mix(deck);
        }
        double elapsed = timer.Elapsed();
        log(logxx::info) << decksCount << " decks generated in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
    }
}

std::vector<Performance::StandardDeck> Performance::PregenerateConvergableDecks()
{
    S_LOG("PregenerateConvergableDecks");
    StandardMixer mixer;
    Patience::PatienceInfo info;
    std::vector<StandardDeck> pregeneratedDecks;
    static const std::size_t decksCount = 1E3;
    pregeneratedDecks.reserve(decksCount);
    auto deck = standard_36_deck::Deck::cards;
    {
        TimeMeasure timer;
        for (std::size_t i = 0; i != decksCount; ++i) {
            generator::Generate(deck, info, mixer, interrupt);
            pregeneratedDecks.push_back(deck);
        }
        double elapsed = timer.Elapsed();
        log(logxx::info) << decksCount << " convergable decks generated in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
    }
    return pregeneratedDecks;
}

void Performance::MediciGenerator()
{
    S_LOG("MediciGenerator");
    StandardMixer mixer;
    Patience::PatienceInfo info;

    auto pregeneratedDecks = PregenerateConvergableDecks();
    auto it = pregeneratedDecks.cbegin();
    auto end = pregeneratedDecks.cend();
    {
        TimeMeasure timer;
        for (; it != end; ++it) {
            auto &deck = *it;
            if (!Patience::Converge(deck, info))
                throw std::logic_error("Convergable deck doesn't converges!");
        }
        double elapsed = timer.Elapsed();
        log(logxx::info) << pregeneratedDecks.size() << " convergable decks converged in " << elapsed << "s: " <<
               pregeneratedDecks.size() / elapsed << " decks per second" << logxx::endl;
    }

}

DeckSelectors Performance::DefaultSelectors()
{
    using namespace standard_36_deck;
    DeckOneSelector targetCard({CardSelector(Card::Suit(Hearts), Card::Rank(Ten), true)}, 19, 24);
    DeckAllSelector ownActions({CardSelector(Card::Rank(Ace), false)}, 3, 7);
    DeckOneSelector firstCard({CardSelector(Card::Rank(Jack), true)}, 0, 0);
    DeckOneSelector secondCard({CardSelector(Card::Rank(Nine), true)}, 1, 1);
    DeckOneSelector thirdCard({CardSelector(Card::Rank(Ace), true), CardSelector(Card::Rank(Ten), true)}, 2, 2);

    DeckSelectors selectors;
    selectors.AddDeckSelector(targetCard);
    selectors.AddDeckSelector(ownActions);
    selectors.AddDeckSelector(firstCard);
    selectors.AddDeckSelector(secondCard);
    selectors.AddDeckSelector(thirdCard);
    return selectors;
}

Performance::CheckOperand Performance::DefaultCheckOperand()
{
    CheckOperand operand(DefaultSelectors());
    return operand;
}

void Performance::MediciWithConditions()
{
    S_LOG("MediciWithConditions");

    StandardMixer mixer;
    auto deck = standard_36_deck::Deck::cards;
    Patience::PatienceInfo info;
    auto checker = DefaultCheckOperand();
    const std::size_t totalDecks = 100;
    TimeMeasure timer;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        generator::Generate(deck, info, mixer, interrupt, checker);
    }
    double elapsed = timer.Elapsed();
    log(logxx::info) << totalDecks << " appropriate decks generated in " << elapsed << "s: " << totalDecks / elapsed << " decks per second" << logxx::endl;
}

void Performance::MediciWithConditionsAndIChing()
{
    S_LOG("MediciWithConditionsAndIChing");

    auto deck = standard_36_deck::Deck::cards;
    Patience::PatienceInfo info;
    i_ching::BalanceChecker iChingChecker;
    auto checker = DefaultCheckOperand();
    StandardMixer mixer;
    
    std::size_t totalDecks = 100;
    std::size_t balanced = 0;
    TimeMeasure timer;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        generator::Generate(deck, info, mixer, interrupt, checker);
        if (iChingChecker.Check(info))
            ++balanced;
    }
    double elapsed = timer.Elapsed();
    if (balanced != 0)
        log(logxx::info) << balanced << " balanced decks" << logxx::endl;
    log(logxx::info) << totalDecks << " appropriate decks generated and checked in " << elapsed << "s: " <<
        totalDecks / elapsed << " decks per second" << logxx::endl;
}

void Performance::IChingBalancedPercent()
{
    S_LOG("IChingBalancedPercent");
    auto deck = standard_36_deck::Deck::cards;
    Patience::PatienceInfo info;
    i_ching::BalanceChecker iChingChecker;
    StandardMixer mixer;

    std::size_t totalDecks = 1E5;
    std::size_t balanced = 0;
    for (std::size_t i = 0; i != totalDecks; ++i) {
        generator::Generate(deck, info, mixer, interrupt);
        if (iChingChecker.Check(info))
            ++balanced;
    }
    log(logxx::info) << static_cast<double>(balanced) / totalDecks * 100.0 << "% balanced decks" << logxx::endl;
}

