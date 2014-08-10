#ifndef CARDS_STANDARD_36_DECK_H
#define CARDS_STANDARD_36_DECK_H

#include "deck.h"

namespace standard_36_deck{

	enum Suits{
		Spades = 0,
		Hearts = 1,
		Diamonds = 2,
		Clubs = 3
	};	
	
	enum Ranks{
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

	typedef ::Deck<36> Deck;
}

#endif /* CARDS_STANDARD_36_DECK_H */
