#include "medici.h"


void Medici::PatienceInfo::Clear(){
	convolutions.clear();
	stationars.clear();
	mobiles.clear();
}

bool Medici::Converges(const PCard& left, const PCard& right){
	return left->suit == right->suit || left->rank == right->rank;
}

void Medici::Mobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info){
	info.mobiles.insert(middleCard);
	if (info.mobiles.find(leftCard) == info.mobiles.end())
		info.stationars.insert(leftCard);
}

std::size_t Medici::Converge(std::vector<PCard>& deck, PatienceInfo& info){
	if (deck.size() <= 2)
		return 0;
	std::size_t convolutions = 0;
	std::size_t rightCardN = 0;
	while (rightCardN != deck.size() && deck.size() > 2){
		if (rightCardN < 2)
			++rightCardN;
		else {
			auto &rightPCard = deck[rightCardN];
			auto middleCardN = rightCardN - 1; auto &middlePCard = deck[middleCardN];
			auto leftCardN  = middleCardN - 1; auto &leftPCard   = deck[leftCardN];
			if (Converges(leftPCard, rightPCard)){
				Mobiles(*leftPCard, *middlePCard, info);
				deck.erase(deck.begin() + leftCardN);
				rightCardN = leftCardN;
				++convolutions;
			} else
				++rightCardN;
		}
	}
	return convolutions;
}

