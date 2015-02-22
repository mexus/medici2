#ifndef I_CHING_I_CHING_H
#define I_CHING_I_CHING_H

#include <cards/standard-36-deck.h>
#include <medici/patience.h>
#include <array>

namespace i_ching {

	enum Line {
		Yang,
		Yin
	};
	typedef std::array<Line, 6> Hexagram;
	typedef std::array<Hexagram, 4> SuitsHexagrams;

	struct BalanceChecker{
		bool operator()(const standard_36_deck::Deck::ArrayType&, const medici::Patience::PatienceInfo&) const;
	};

	class BalanceAndSuitChecker{
	public:
		BalanceAndSuitChecker(const Card::Suit& suit, const Hexagram&);
		bool operator()(const standard_36_deck::Deck::ArrayType&, const medici::Patience::PatienceInfo&) const;
	private:
		std::uint_fast8_t suit;
		Hexagram hexagram;
	};

	SuitsHexagrams CalculateHexagrams(const medici::Patience::PatienceInfo& info);


}

#endif /* I_CHING_I_CHING_H */
