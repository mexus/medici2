#include <medici/patience.h>

using cards::Card;

namespace {
inline bool CheckConvergence(const Card& left, const Card& right) {
    return left.suit == right.suit || left.rank == right.rank;
}

inline void PopulateMobiles(const Card& leftCard, const Card& middleCard,
                            medici::PatienceInfo& info) {
    info.mobiles.insert(middleCard);
    if (info.mobiles.find(leftCard) == info.mobiles.end())
        info.stationars.insert(leftCard);
}

std::size_t ConvergeDeckPart(std::vector<Card>& deck, medici::PatienceInfo& info) {
    if (deck.size() <= 2)
        return 0;
    auto rightCardIt = deck.end() - 1;
    std::size_t convolutions = 0;
    while (rightCardIt != deck.end() && deck.size() > 2) {
        if (std::distance(deck.begin(), rightCardIt) < 2)
            ++rightCardIt;
        else {
            auto middleCardIt = rightCardIt - 1;
            auto leftCardIt = middleCardIt - 1;
            if (CheckConvergence(*leftCardIt, *rightCardIt)) {
                PopulateMobiles(*leftCardIt, *middleCardIt, info);
                rightCardIt = deck.erase(leftCardIt);
                ++convolutions;
            } else
                ++rightCardIt;
        }
    }
    return convolutions;
}
}

namespace medici {

void PatienceInfo::Clear() {
    convolutions.clear();
    stationars.clear();
    mobiles.clear();
}

bool TryToConverge(const std::vector<Card>& cards, PatienceInfo& info) {
    std::vector<Card> deck;
    info.Clear();
    for (std::size_t i = 0; i != cards.size(); ++i) {
        const Card& current = cards[i];
        deck.push_back(current);
        std::size_t convolutions = ConvergeDeckPart(deck, info);
        if (convolutions != 0)
            info.convolutions[current] = convolutions;
    }
    if (deck.size() != 2) {
        return false;
    }
    /* Last card is always a mobile */
    info.mobiles.insert(cards.back());
    return true;
}
}
