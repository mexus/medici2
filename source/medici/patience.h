#ifndef MEDICI_PATIENCE_H
#define MEDICI_PATIENCE_H

#include <cards/deck.h>
#include <map>
#include <set>
#include <vector>

namespace medici{

	class Patience {
	public:
		struct PatienceInfo {
			std::map<Card, std::size_t> convolutions;
			std::set<Card> mobiles, stationars;
	
			void Clear();
		};
		template<std::size_t N>
		static bool Converge(const std::array<Card, N>&, PatienceInfo&);
	private:
		static std::size_t Converge(std::vector<Card>& deck, PatienceInfo&);
		static void Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
		static bool Converges(const Card& left, const Card& right);
	
	};

}

#include "patience.hpp"

#endif /* MEDICI_PATIENCE_H */
