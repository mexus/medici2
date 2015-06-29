#ifndef CARDS_CARD_H
#define CARDS_CARD_H

#include <type_traits>
#include <cstdint>

struct Card {
    std::uint_fast8_t suit;
    std::uint_fast8_t rank;
};

bool operator<(const Card& lhs, const Card& rhs);

#endif /* CARDS_CARD_H */
