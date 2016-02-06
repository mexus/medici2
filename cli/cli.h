#pragma once

#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>
#include <mixer/factory.h>
#include <calculator/manager.h>

class Cli {
public:
    Cli(MixersFactory::MixerType mixer_type);
    void Run();

private:
    static constexpr size_t N = 36;
    MixersFactory mixers_factory_;
    calculator::Manager calculator_;

    static CardSelector AnySuit(standard_36_deck::Ranks rank, bool straight = true);
};
