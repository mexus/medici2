#ifndef CARDS_CARD_SELECTOR_H
#define CARDS_CARD_SELECTOR_H

#include "deck.h"

class CardSelector {
public:
	CardSelector(const Card::Suit& suit, const Card::Rank& rank, bool straight);
	CardSelector(const Card::Suit& suit, bool straight);
	CardSelector(const Card::Rank& rank, bool straight);

	bool Check(const Card&) const;

	const Card::Suit suit;
	const Card::Rank rank;
	const bool suitSet;
	const bool rankSet;
	const bool straight;
};

#endif /* CARDS_CARD_SELECTOR_H */
