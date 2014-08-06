#include "medici.h"


void Medici::PatienceInfo::Clear(){
	convolutions.clear();
	stationars.clear();
	mobiles.clear();
}

bool Medici::Converges(const Card& left, const Card& right){
	return left.suit == right.suit || left.rank == right.rank;
}

void Medici::Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info){
	info.mobiles.insert(middleCard);
	if (info.mobiles.find(leftCard) == info.mobiles.end())
		info.stationars.insert(leftCard);
}

std::size_t Medici::Converge(std::vector<Card>& deck, PatienceInfo& info){
	if (deck.size() <= 2)
		return 0;
	auto rightCardIt = deck.end() - 1;
	std::size_t convolutions = 0;
	while (rightCardIt != deck.end() && deck.size() > 2){
		auto middleCardIt = rightCardIt - 1;
		auto leftCardIt  = middleCardIt - 1;
		if (Converges(*leftCardIt, *rightCardIt)){
			Mobiles(*leftCardIt, *middleCardIt, info);
			rightCardIt = deck.erase(leftCardIt);
			++convolutions;
		} else
			++rightCardIt;
	}
	return convolutions;
}

