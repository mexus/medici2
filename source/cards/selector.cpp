#include "selector.h"

CardSelector::CardSelector(std::uint_fast8_t suit, std::uint_fast8_t rank) : suit(suit), rank(rank), suitSet(true), rankSet(true){
}

CardSelector::CardSelector(std::uint_fast8_t rank, const OnlyRank&) : rank(rank), suitSet(false), rankSet(true){
}

CardSelector::CardSelector(std::uint_fast8_t suit, const OnlySuit&) : suit(suit), suitSet(true), rankSet(false){
}

bool CardSelector::Check(const Card& card) const {
	return
		(!rankSet || rank == card.rank) &&
		(!suitSet || suit == card.suit);
}

