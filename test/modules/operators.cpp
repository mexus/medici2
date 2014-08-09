#include "operators.h"

std::ostream& operator<<(std::ostream& s, const Card& card){
	return s << "suit #" << (int)card.suit.value << " rank #" << (int)card.rank.value;
}

std::ostream& operator<<(std::ostream& s, const CardSelector& cardSelector){
	s << "{";
	if (cardSelector.straight)
		s << "straight";
	else
		s << "inverse";
	s << ", ";
	bool printSeparator = false;
	if (cardSelector.suitSet){
		s << "suit #" << static_cast<int>(cardSelector.suit.value);
		printSeparator = true;
	}
	if (cardSelector.rankSet){
		if (printSeparator)
			s << ", ";
		s << "rank #" << static_cast<int>(cardSelector.rank.value);
	}
	s << "}";
	return s;
}


