#include "card.h"

bool operator<(const Card& lhs, const Card& rhs) {
    if (lhs.suit == rhs.suit)
        return lhs.rank < rhs.rank;
    else
        return lhs.suit < rhs.suit;
}
