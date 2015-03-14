#ifndef MEDICI_MEDICI_HPP
#define MEDICI_MEDICI_HPP

#include "patience.h"

namespace medici{

    template<std::size_t N>
    bool Patience::Converge(const std::array<Card, N>& cards, PatienceInfo& info){
        static_assert(N >= 3, "Deck should be at least 3 cards large!");
        std::vector<Card> deck;
        info.Clear();
        for (std::size_t i = 0; i != N; ++i){
            const Card& current = cards[i];
            deck.push_back(current);
            std::size_t convolutions = Converge(deck, info);
            if (convolutions != 0)
                info.convolutions[current] = convolutions;
        }
        return deck.size() == 2;
    }

}

#endif /* MEDICI_MEDICI_HPP */
