#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <atomic>
#include <logxx/logxx.h>
#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>
#include <mixer/mixer.h>

class Performance {
public:
    void Run();
private:
    typedef standard_36_deck::Deck StandardDeck;
    typedef StandardDeck::ArrayType StandardDeckArray;

    class CheckOperand {
    public:
        CheckOperand(DeckSelectors&&);
        bool operator()(const StandardDeckArray&) const;
    private:
        DeckSelectors deckSelectors;
    };

    static logxx::Log cLog;

    static void Mixing();
    static std::vector<StandardDeckArray> PregenerateConvergableDecks();
    static void MediciGenerator();
    static void MediciWithConditions();
    static void MediciWithConditionsAndIChing();
    static void IChingBalancedPercent();

    static DeckSelectors DefaultSelectors();
    static CheckOperand DefaultCheckOperand();
    
    typedef std::unique_ptr<MixerInterface<Card, StandardDeck::N()>> StandardGenerator;
    static StandardGenerator GetMixer(std::uint_fast32_t seed = 0);
};

#endif /* PERFORMANCE_H */

