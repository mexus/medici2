#include "test_medici.h"

logxx::Log TestMedici::cLog("TestMedici");

TestMedici::TestMedici() : TestFW("medici"){
}

bool TestMedici::Tests(){
	using namespace standard_36_deck;
	ArrayType convergingDeck1 { {{Jack, Spades}, {Nine, Hearts}, {Ten, Hearts}, {Queen, Spades}, {Nine, Spades}, {Seven, Diamonds}, {Queen, Hearts}, {Jack, Hearts}, {Queen, Diamonds}, {Ten, Diamonds}, {Nine, Diamonds}, {Six, Spades}, {Ace, Spades}, {Seven, Hearts}, {Ten, Spades}, {Queen, Clubs}, {Six, Clubs}, {King, Hearts}, {King, Spades}, {Ace, Clubs}, {Six, Hearts}, {King, Diamonds}, {Ace, Hearts}, {Eight, Spades}, {Eight, Hearts}, {Seven, Spades}, {Nine, Clubs}, {Seven, Clubs}, {Eight, Diamonds}, {King, Clubs}, {Eight, Clubs}, {Jack, Clubs}, {Jack, Diamonds}, {Six, Diamonds}, {Ten, Clubs}, {Ace, Diamonds}} };
	Medici::PatienceInfo deck1Info;
	deck1Info.stationars = {{Jack, Spades}, {Ten, Hearts}, {Nine, Spades}, {Seven, Diamonds}, {Queen, Hearts}, {Queen, Diamonds}, {Six, Spades}, {Ace, Spades}, {Queen, Clubs}, {Six, Clubs}, {King, Spades}, {Six, Hearts}, {Ace, Hearts}, {Seven, Spades}, {Seven, Clubs}, {Jack, Clubs}, {Six, Diamonds}};

	ArrayType notConvergingDeck2 { {{Ten, Hearts}, {Eight, Clubs}, {Ace, Hearts}, {Jack, Clubs}, {Seven, Spades}, {Ten, Clubs}, {Nine, Spades}, {Seven, Hearts}, {Nine, Hearts}, {Ace, Diamonds}, {Six, Hearts}, {Queen, Spades}, {Eight, Hearts}, {King, Diamonds}, {Ace, Spades}, {Ten, Spades}, {Jack, Diamonds}, {Eight, Spades}, {Seven, Clubs}, {Nine, Diamonds}, {Six, Spades}, {Queen, Hearts}, {King, Hearts}, {Jack, Spades}, {Jack, Hearts}, {King, Spades}, {Nine, Clubs}, {Queen, Diamonds}, {Ten, Diamonds}, {Eight, Diamonds}, {Six, Clubs}, {Queen, Clubs}, {Seven, Diamonds}, {Ace, Clubs}, {Six, Diamonds}, {King, Clubs}} };

	return Test(notConvergingDeck2, false) && Test(convergingDeck1, true, deck1Info);
}

namespace {
	std::ostream& operator<<(std::ostream& s, const Card& card){
		return s << "suit #" << (int)card.suit << " rank #" << (int)card.rank;
	}
}

bool TestMedici::Test(const ArrayType& deck, bool etalonConverges, const Medici::PatienceInfo& etalonInfo){
	S_LOG("Test");
	Medici::PatienceInfo resultInfo;
	bool resultConverges = Medici::Converge<DeckType::N()>(deck, resultInfo);

	bool res;
	if (resultConverges != etalonConverges){
		auto &s = log(logxx::error) << "Deck should ";
		if (!etalonConverges)
			s << "not ";
		s << "converge, but it does ";
		if (!resultConverges)
			s << "not";
		s << logxx::endl;
		res = false;
	} else if (resultConverges)
		res = Compare(resultInfo, etalonInfo);
	else
		res = true;
	if (!res){
		log(logxx::error) << "Failed on {" << deck << "}" << logxx::endl;
	}
	return res;
}

bool TestMedici::Compare(const Medici::PatienceInfo& etalon, const Medici::PatienceInfo& result){
	S_LOG("Compare");
	if (!TestFW::Compare(etalon.convolutions, result.convolutions)){
		log(logxx::error) << "Convolutions check failed" << logxx::endl;
		return false;
	} else if (!TestFW::Compare(etalon.stationars, result.stationars)){
		log(logxx::error) << "Stationars check failed" << logxx::endl;
		return false;
	} else
		return true;
}

