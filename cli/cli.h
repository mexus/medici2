#pragma once

#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>
#include <mixer/factory.h>

class Cli {
public:
    Cli();
    void Run();

private:
    static constexpr std::size_t N = 36;
    static CardSelectorConfigurator configurator;
    MixersFactory mixersFactory;

    void Mixing() const;
    std::vector<std::vector<Card>> PregenerateConvergableDecks() const;
    void MediciGenerator() const;
    void MediciWithConditions() const;
    void MediciWithConditionsAndIChing() const;
    void IChingBalancedPercent() const;

    static DeckSelectors DefaultSelectors();
    static CheckOperand DefaultCheckOperand();
    static CardSelector SelectorAnyRank(std::uint_fast8_t suit, bool straight = true);
    static CardSelector SelectorAnySuit(std::uint_fast8_t rank, bool straight = true);
    static CardSelector Selector(std::uint_fast8_t suit, std::uint_fast8_t rank,
                                 bool straight = true);

    typedef std::unique_ptr<MixerInterface<Card>> StandardGenerator;
    StandardGenerator GetMixer(std::uint_fast32_t seed = 0) const;
};
