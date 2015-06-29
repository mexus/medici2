#ifndef CARDS_STANDARD_36_DECK_H
#define CARDS_STANDARD_36_DECK_H

#include <vector>
#include "card.h"

namespace standard_36_deck {

    enum Suits : std::uint_fast8_t {
        Spades,
        Hearts,
        Diamonds,
        Clubs
    };  
    
    enum Ranks : std::uint_fast8_t {
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace
    };

    static const std::vector<Card> kDeck {{
        {standard_36_deck::Spades, standard_36_deck::Six},     // 0
        {standard_36_deck::Spades, standard_36_deck::Seven},   // 1
        {standard_36_deck::Spades, standard_36_deck::Eight},   // 2
        {standard_36_deck::Spades, standard_36_deck::Nine},    // 3
        {standard_36_deck::Spades, standard_36_deck::Ten},     // 4
        {standard_36_deck::Spades, standard_36_deck::Jack},    // 5
        {standard_36_deck::Spades, standard_36_deck::Queen},   // 6
        {standard_36_deck::Spades, standard_36_deck::King},    // 7
        {standard_36_deck::Spades, standard_36_deck::Ace},     // 8

        {standard_36_deck::Hearts, standard_36_deck::Six},     // 9
        {standard_36_deck::Hearts, standard_36_deck::Seven},   // 10
        {standard_36_deck::Hearts, standard_36_deck::Eight},   // 11
        {standard_36_deck::Hearts, standard_36_deck::Nine},    // 12
        {standard_36_deck::Hearts, standard_36_deck::Ten},     // 13
        {standard_36_deck::Hearts, standard_36_deck::Jack},    // 14
        {standard_36_deck::Hearts, standard_36_deck::Queen},   // 15
        {standard_36_deck::Hearts, standard_36_deck::King},    // 16
        {standard_36_deck::Hearts, standard_36_deck::Ace},     // 17

        {standard_36_deck::Diamonds, standard_36_deck::Six},   // 18
        {standard_36_deck::Diamonds, standard_36_deck::Seven}, // 19
        {standard_36_deck::Diamonds, standard_36_deck::Eight}, // 20
        {standard_36_deck::Diamonds, standard_36_deck::Nine},  // 21
        {standard_36_deck::Diamonds, standard_36_deck::Ten},   // 22
        {standard_36_deck::Diamonds, standard_36_deck::Jack},  // 23
        {standard_36_deck::Diamonds, standard_36_deck::Queen}, // 24
        {standard_36_deck::Diamonds, standard_36_deck::King},  // 25
        {standard_36_deck::Diamonds, standard_36_deck::Ace},   // 26

        {standard_36_deck::Clubs, standard_36_deck::Six},      // 27
        {standard_36_deck::Clubs, standard_36_deck::Seven},    // 28
        {standard_36_deck::Clubs, standard_36_deck::Eight},    // 29
        {standard_36_deck::Clubs, standard_36_deck::Nine},     // 30
        {standard_36_deck::Clubs, standard_36_deck::Ten},      // 31
        {standard_36_deck::Clubs, standard_36_deck::Jack},     // 32
        {standard_36_deck::Clubs, standard_36_deck::Queen},    // 33
        {standard_36_deck::Clubs, standard_36_deck::King},     // 34
        {standard_36_deck::Clubs, standard_36_deck::Ace}       // 35
    }};

}

#endif /* CARDS_STANDARD_36_DECK_H */
