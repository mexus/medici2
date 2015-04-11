#ifndef MEDICI_PATIENCE_H
#define MEDICI_PATIENCE_H

#include <cards/deck.h>
#include <map>
#include <set>
#include <vector>

namespace medici {

    class Patience {
    public:
        struct PatienceInfo {
            std::map<Card, std::size_t> convolutions;
            std::set<Card> mobiles, stationars;
    
            void Clear();
        };

        template<std::size_t N>
        static bool Converge(const std::array<Card, N>& cards, PatienceInfo& info)
        {
            static_assert(N >= 3, "Deck should be at least 3 cards large!");
            std::vector<Card> deck;
            info.Clear();
            for (std::size_t i = 0; i != N; ++i) {
                const Card& current = cards[i];
                deck.push_back(current);
                std::size_t convolutions = Converge(deck, info);
                if (convolutions != 0)
                    info.convolutions[current] = convolutions;
            }
            return deck.size() == 2;
        }
    private:
        static std::size_t Converge(std::vector<Card>& deck, PatienceInfo&);
        static void Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
        static bool Converges(const Card& left, const Card& right);
    
    };

}

#endif /* MEDICI_PATIENCE_H */
