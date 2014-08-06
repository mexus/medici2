#include "deck.h"


bool Card::operator<(const Card& other) const{
	if (suit == other.suit)
		return rank < other.rank;
	else
		return suit < other.suit;
}

