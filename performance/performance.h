#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <atomic>
#include <logxx/logxx.h>
#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>
#include <mixer/factory.h>

class Performance {
public:
    void Run(MixersFactory::MixerType);
private:
    MixersFactory mixersFactory;

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

    void Mixing() const;
    std::vector<StandardDeckArray> PregenerateConvergableDecks() const;
    void MediciGenerator() const;
    void MediciWithConditions() const;
    void MediciWithConditionsAndIChing() const;
    void IChingBalancedPercent() const;

    static DeckSelectors DefaultSelectors();
    static CheckOperand DefaultCheckOperand();
    
    typedef std::unique_ptr<MixerInterface<Card, StandardDeck::N()>> StandardGenerator;
    StandardGenerator GetMixer(std::uint_fast32_t seed = 0) const;
};

#endif /* PERFORMANCE_H */

