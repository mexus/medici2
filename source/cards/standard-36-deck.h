#ifndef CARDS_STANDARD_36_DECK_H
#define CARDS_STANDARD_36_DECK_H

#include <vector>
#include "card.h"

namespace standard_36_deck {

enum Suits : std::uint_fast8_t { Spades, Hearts, Diamonds, Clubs };

enum Ranks : std::uint_fast8_t { Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

static const std::vector<Card> kDeck{{
    {Spades, Six},    // 0
    {Spades, Seven},  // 1
    {Spades, Eight},  // 2
    {Spades, Nine},   // 3
    {Spades, Ten},    // 4
    {Spades, Jack},   // 5
    {Spades, Queen},  // 6
    {Spades, King},   // 7
    {Spades, Ace},    // 8

    {Hearts, Six},    // 9
    {Hearts, Seven},  // 10
    {Hearts, Eight},  // 11
    {Hearts, Nine},   // 12
    {Hearts, Ten},    // 13
    {Hearts, Jack},   // 14
    {Hearts, Queen},  // 15
    {Hearts, King},   // 16
    {Hearts, Ace},    // 17

    {Diamonds, Six},    // 18
    {Diamonds, Seven},  // 19
    {Diamonds, Eight},  // 20
    {Diamonds, Nine},   // 21
    {Diamonds, Ten},    // 22
    {Diamonds, Jack},   // 23
    {Diamonds, Queen},  // 24
    {Diamonds, King},   // 25
    {Diamonds, Ace},    // 26

    {Clubs, Six},    // 27
    {Clubs, Seven},  // 28
    {Clubs, Eight},  // 29
    {Clubs, Nine},   // 30
    {Clubs, Ten},    // 31
    {Clubs, Jack},   // 32
    {Clubs, Queen},  // 33
    {Clubs, King},   // 34
    {Clubs, Ace}     // 35
}};
}

#endif /* CARDS_STANDARD_36_DECK_H */
