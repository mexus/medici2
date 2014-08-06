#ifndef CARDS_DECK_H
#define CARDS_DECK_H

#include <array>
#include <cstdint>

struct Card{
	std::uint_fast8_t suit;
	std::uint_fast8_t rank;
	bool operator<(const Card& other) const;
};

template<int N>
struct Deck{
	typedef std::array<Card, N> ArrayType;
	static ArrayType cards;
};

#endif /* CARDS_DECK_H */
