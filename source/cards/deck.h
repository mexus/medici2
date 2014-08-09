#ifndef CARDS_DECK_H
#define CARDS_DECK_H

#include <array>
#include <cstdint>

struct Card{
	struct Suit;
	struct Rank;

	struct Suit{
		Suit(std::uint_fast8_t);
		Suit(const Rank&) = delete;
		Suit();

		std::uint_fast8_t value;
		bool operator==(const Suit&) const;
		bool operator<(const Suit&) const;
	};

	struct Rank{
		Rank(std::uint_fast8_t);
		Rank(const Suit&) = delete;
		Rank();

		std::uint_fast8_t value;
		bool operator==(const Rank&) const;
		bool operator<(const Rank&) const;
	};

	Suit suit;
	Rank rank;
	bool operator<(const Card& other) const;
};

template<std::size_t n>
struct Deck{
	typedef std::array<Card, n> ArrayType;
	static const ArrayType cards;

	static constexpr std::size_t N() {return n;}
};

#endif /* CARDS_DECK_H */
