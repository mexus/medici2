#include "standard-36-deck.h"

template<>
const standard_36_deck::Deck::ArrayType standard_36_deck::Deck::cards {{
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

