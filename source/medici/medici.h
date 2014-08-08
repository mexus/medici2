#ifndef MEDICI_MEDICI_H
#define MEDICI_MEDICI_H

#include <cards/deck.h>
#include <map>
#include <set>
#include <vector>
#include <memory>

class Medici {
public:
	struct PatienceInfo {
		std::map<Card, std::size_t> convolutions;
		std::set<Card> mobiles, stationars;

		void Clear();
	};
	template<std::size_t N>
	static bool Converge(const std::array<Card, N>&, PatienceInfo&);
private:
	typedef const Card* PCard;

	static std::size_t Converge(std::vector<PCard>& deck, PatienceInfo&);
	static void Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
	static bool Converges(const PCard& left, const PCard& right);

};

#include "medici.hpp"

#endif /* MEDICI_MEDICI_H */
