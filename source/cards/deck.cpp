#include "deck.h"


Card::Suit::Suit() :
    value(0)
{
}

Card::Suit::Suit(std::uint_fast8_t v) :
    value(v)
{
}

Card::Rank::Rank() :
    value(0)
{
}

Card::Rank::Rank(std::uint_fast8_t v) :
    value(v)
{
}

bool Card::Suit::operator==(const Suit& other) const
{
    return value == other.value;
}

bool Card::Suit::operator<(const Suit& other) const
{
    return value < other.value;
}

bool Card::Rank::operator==(const Rank& other) const
{
    return value == other.value;
}

bool Card::Rank::operator<(const Rank& other) const
{
    return value < other.value;
}

bool Card::operator<(const Card& other) const
{
    if (suit == other.suit)
        return rank < other.rank;
    else
        return suit < other.suit;
}

