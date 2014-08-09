#ifndef CARDS_SELECTOR_H
#define CARDS_SELECTOR_H

#include "deck.h"

class CardSelector {
public:
	struct OnlyRank{};
	struct OnlySuit{};

	CardSelector(std::uint_fast8_t suit, std::uint_fast8_t rank);
	CardSelector(std::uint_fast8_t rank, const OnlyRank&);
	CardSelector(std::uint_fast8_t suit, const OnlySuit&);

	bool Check(const Card&) const;
private:
	Card::Suit suit;
	Card::Rank rank;
	bool suitSet;
	bool rankSet;
};

#endif /* CARDS_SELECTOR_H */
