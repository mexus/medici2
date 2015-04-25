#ifndef CARDS_DECK_H
#define CARDS_DECK_H

#include <array>
#include "card.h"

template<std::size_t n>
struct Deck {
    typedef std::array<Card, n> ArrayType;
    static const ArrayType cards;

    static constexpr std::size_t N()
    {
        return n;
    }
};

#endif /* CARDS_DECK_H */
