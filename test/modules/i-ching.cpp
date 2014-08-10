#include "i-ching.h"
#include "operators.h"

logxx::Log TestIChing::cLog("TestIChing");
using namespace i_ching;
using namespace medici;
using namespace standard_36_deck;

TestIChing::TestIChing() : TestFW("i-ching"){
}

bool TestIChing::Tests(){
	return TestBalance() && TestBalanceAndSuit();
}

bool TestIChing::TestBalance(){
	ArrayType balancedDeck {{
		{Hearts, Jack},	
		{Hearts, Nine},	
		{Hearts, Ten},	
		{Diamonds, Queen},	
		{Clubs, Seven},	
		{Hearts, Seven},	
		{Diamonds, Jack},	
		{Spades, Jack},	
		{Hearts, Six},	
		{Hearts, Queen},	
		{Diamonds, Seven},	
		{Spades, Ace},	
		{Spades, Queen},	
		{Spades, Ten},	
		{Clubs, Jack},	
		{Clubs, Nine},	
		{Diamonds, Nine},	
		{Diamonds, Ten},	
		{Diamonds, Six},	
		{Clubs, Queen},	
		{Diamonds, Ace},	
		{Clubs, Ten},	
		{Clubs, Six},	
		{Diamonds, King},	
		{Hearts, Ace},	
		{Clubs, Eight},	
		{Hearts, Eight},	
		{Spades, Six},	
		{Hearts, King},	
		{Clubs, King},	
		{Spades, Nine},	
		{Spades, King},	
		{Diamonds, Eight},	
		{Spades, Seven},	
		{Clubs, Ace},	
		{Spades, Eight} 
	}} ;
	ArrayType unbalancedDeck {{
		{Spades, Jack},	
		{Hearts, Nine},	
		{Diamonds, Ace},	
		{Clubs, Six},	
		{Clubs, Queen},	
		{Spades, Queen},	
		{Spades, Seven},	
		{Clubs, Jack},	
		{Diamonds, Queen},	
		{Spades, Six},	
		{Diamonds, King},	
		{Spades, Ten},	
		{Diamonds, Ten},	
		{Hearts, King},	
		{Clubs, King},	
		{Hearts, Ten},	
		{Spades, Nine},	
		{Diamonds, Eight},	
		{Clubs, Ten},	
		{Clubs, Ace},	
		{Hearts, Six},	
		{Hearts, Queen},	
		{Clubs, Eight},	
		{Hearts, Ace},	
		{Diamonds, Six},	
		{Diamonds, Seven},	
		{Clubs, Nine},	
		{Hearts, Eight},	
		{Clubs, Seven},	
		{Diamonds, Nine},	
		{Spades, Ace},	
		{Diamonds, Jack},	
		{Spades, Eight},	
		{Hearts, Seven},	
		{Spades, King},	
		{Hearts, Jack}
	}};

	return TestBalance(balancedDeck, true) && TestBalance(unbalancedDeck, false);
}

bool TestIChing::TestBalance(const ArrayType& deck, bool balancedEtalon){
	S_LOG("TestBalance");
	Patience::PatienceInfo info;
	if (!Patience::Converge(deck, info)){
		log(logxx::error) << "Deck doesn't converge!\n" << deck << logxx::endl;
		return false;
	} else {
		BalanceChecker checker;
		bool balancedResult = checker(deck, info);
		if (balancedResult != balancedEtalon){
			auto &s = log(logxx::error) << "Deck is";
			if (!balancedResult)
				s << " not";
			s << " balanced, but it should";
			if (!balancedEtalon)
				s << " not";
			s << " be";
			s << "\nDeck: " << deck << logxx::endl;
			return false;
		} else
			return true;
	}
}

bool TestIChing::TestBalanceAndSuit(){
	return false;
}


