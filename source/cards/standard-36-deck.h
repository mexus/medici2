#ifndef CARDS_STANDARD_36_DECK_H
#define CARDS_STANDARD_36_DECK_H

#include "deck.h"

namespace standard_36_deck{

	enum Suits{
		Spades,
		Hearts,
		Diamonds,
		Clubs
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

	template<>
	standard_36_deck::Deck::ArrayType standard_36_deck::Deck::cards {{
		{standard_36_deck::Spades, standard_36_deck::Six}, {standard_36_deck::Spades, standard_36_deck::Seven},
		{standard_36_deck::Spades, standard_36_deck::Eight}, {standard_36_deck::Spades, standard_36_deck::Nine},
		{standard_36_deck::Spades, standard_36_deck::Ten}, {standard_36_deck::Spades, standard_36_deck::Jack},
		{standard_36_deck::Spades, standard_36_deck::Queen}, {standard_36_deck::Spades, standard_36_deck::King},
		{standard_36_deck::Spades, standard_36_deck::Ace},

		{standard_36_deck::Hearts, standard_36_deck::Six}, {standard_36_deck::Hearts, standard_36_deck::Seven},
		{standard_36_deck::Hearts, standard_36_deck::Eight}, {standard_36_deck::Hearts, standard_36_deck::Nine},
		{standard_36_deck::Hearts, standard_36_deck::Ten}, {standard_36_deck::Hearts, standard_36_deck::Jack},
		{standard_36_deck::Hearts, standard_36_deck::Queen}, {standard_36_deck::Hearts, standard_36_deck::King},
		{standard_36_deck::Hearts, standard_36_deck::Ace},

		{standard_36_deck::Diamonds, standard_36_deck::Six}, {standard_36_deck::Diamonds, standard_36_deck::Seven},
		{standard_36_deck::Diamonds, standard_36_deck::Eight}, {standard_36_deck::Diamonds, standard_36_deck::Nine},
		{standard_36_deck::Diamonds, standard_36_deck::Ten}, {standard_36_deck::Diamonds, standard_36_deck::Jack},
		{standard_36_deck::Diamonds, standard_36_deck::Queen}, {standard_36_deck::Diamonds, standard_36_deck::King},
		{standard_36_deck::Diamonds, standard_36_deck::Ace},

		{standard_36_deck::Clubs, standard_36_deck::Six}, {standard_36_deck::Clubs, standard_36_deck::Seven},
		{standard_36_deck::Clubs, standard_36_deck::Eight}, {standard_36_deck::Clubs, standard_36_deck::Nine},
		{standard_36_deck::Clubs, standard_36_deck::Ten}, {standard_36_deck::Clubs, standard_36_deck::Jack},
		{standard_36_deck::Clubs, standard_36_deck::Queen}, {standard_36_deck::Clubs, standard_36_deck::King},
		{standard_36_deck::Clubs, standard_36_deck::Ace}
}};

#endif /* CARDS_STANDARD_36_DECK_H */
