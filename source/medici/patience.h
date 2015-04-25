#ifndef MEDICI_PATIENCE_H
#define MEDICI_PATIENCE_H

#include <cards/deck.h>
#include <map>
#include <set>
#include <vector>

namespace medici {

    struct PatienceInfo {
        std::map<Card, std::size_t> convolutions;
        std::set<Card> mobiles, stationars;
        void Clear();
    };

    std::size_t ConvergeDeck(std::vector<Card>& deck, PatienceInfo&);
    void PopulateMobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
    bool CheckConvergence(const Card& left, const Card& right);

    template<std::size_t N>
    bool ConvergeDeck(const std::array<Card, N>& cards, PatienceInfo& info)
    {
        static_assert(N >= 3, "Deck should be at least 3 cards large!");
        std::vector<Card> deck;
        info.Clear();
        for (std::size_t i = 0; i != N; ++i) {
            const Card& current = cards[i];
            deck.push_back(current);
            std::size_t convolutions = ConvergeDeck(deck, info);
            if (convolutions != 0)
                info.convolutions[current] = convolutions;
        }
        return deck.size() == 2;
    }

}

#endif /* MEDICI_PATIENCE_H */
