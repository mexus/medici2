#include "card-selector.h"

CardSelector::CardSelector(const Card::Suit& suit, const Card::Rank& rank, bool straight) : suit(suit), rank(rank), suitSet(true), rankSet(true), straight(straight) {
}

CardSelector::CardSelector(const Card::Rank& rank, bool straight) : rank(rank), suitSet(false), rankSet(true), straight(straight){
}

CardSelector::CardSelector(const Card::Suit& suit, bool straight) : suit(suit), suitSet(true), rankSet(false), straight(straight){
}

bool CardSelector::Check(const Card& card) const {
	bool match = 
		(!rankSet || rank == card.rank) &&
		(!suitSet || suit == card.suit);
	return (straight == match);
}

