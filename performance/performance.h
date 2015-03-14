#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <medici/generator.h>
#include <logxx/logxx.h>
#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>

class Performance {
public:
    void Run();
private:
    typedef standard_36_deck::Deck::ArrayType StandardDeck;
    typedef Mixer<Card, standard_36_deck::Deck::N()> StandardMixer;

    class CheckOperand {
    public:
        CheckOperand(DeckSelectors&&);
        bool operator()(const StandardDeck&) const;
    private:
        DeckSelectors deckSelectors;
    };

    static logxx::Log cLog;

    static std::atomic_bool interrupt;

    static void Mixing();
    static std::vector<StandardDeck> PregenerateConvergableDecks();
    static void MediciGenerator();
    static void MediciWithConditions();
    static void MediciWithConditionsAndIChing();
    static void IChingBalancedPercent();

    static DeckSelectors DefaultSelectors();
    static CheckOperand DefaultCheckOperand();
};

#endif /* PERFORMANCE_H */

