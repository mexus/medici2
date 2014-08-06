#ifndef MEDICI_MEDICI_H
#define MEDICI_MEDICI_H

#include <cards/deck.h>
#include <map>
#include <set>
#include <vector>

class Medici {
public:
	struct PatienceInfo {
		std::map<Card, std::size_t> convolutions;
		std::set<Card> mobiles, stationars;

		void Clear();
	};
	template<int N>
	static bool Converge(const std::array<Card, N>&, PatienceInfo&);
private:
	static std::size_t Converge(std::vector<Card>& deck, PatienceInfo&);
	static void Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
	static bool Converges(const Card& left, const Card& right);

};

#include "medici.hpp"

#endif /* MEDICI_MEDICI_H */
