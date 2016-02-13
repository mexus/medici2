#include "card.h"

bool operator<(const Card& lhs, const Card& rhs) {
    if (lhs.suit == rhs.suit)
        return lhs.rank < rhs.rank;
    else
        return lhs.suit < rhs.suit;
}

bool operator==(const Card& lhs, const Card& rhs) {
    return lhs.rank == rhs.rank && lhs.suit == rhs.suit;
}

bool operator!=(const Card& lhs, const Card& rhs) {
    return !(lhs == rhs);
}
