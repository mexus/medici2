#include "i-ching.h"
#include "operators.h"

logxx::Log TestIChing::cLog("TestIChing");
using namespace i_ching;
using namespace medici;
using namespace standard_36_deck;

TestIChing::TestIChing() : TestFW("i-ching"){
}

bool TestIChing::Tests(){
	return TestCalculation() && TestBalance() && TestBalanceAndSuit();
}

bool TestIChing::TestCalculation(){
	S_LOG("TestCalculation");
	Patience::PatienceInfo info1, info2;
	info1.stationars = {
		{Spades, Jack},	
		{Spades, Eight},
		{Hearts, Six},
		{Hearts, Nine},	
		{Hearts, Ten},
		{Hearts, Jack},
		{Hearts, Queen},
		{Hearts, King},
		{Diamonds, Six},
		{Diamonds, Seven},
		{Diamonds, Eight},
		{Diamonds, Nine},
		{Diamonds, Jack},
		{Diamonds, Ace},
		{Clubs, Seven},
		{Clubs, Jack},
	};
	info2.stationars = {
		{Spades, Ten},
		{Spades, Jack},
		{Spades, Queen},
		{Spades, King},
		{Hearts, Jack},
		{Hearts, Queen},
		{Diamonds, Jack},
		{Diamonds, Queen},
		{Diamonds, Ace},
		{Clubs, Nine},
		{Clubs, Jack},
		{Clubs, King},
	};
	SuitsHexagrams etalonHexagrams1 {{
		{{Yang, Yang, Yang, Yang, Yang, Yang}},  // Spades
		{{Yin , Yin , Yin , Yin , Yin , Yang }}, // Hearts
		{{Yin , Yin , Yang, Yang, Yang, Yin }},  // Diamonds
		{{Yang, Yang, Yang, Yang, Yang, Yang}}   // Clubs
	}};
	SuitsHexagrams etalonHexagrams2 {{
		{{Yang, Yang, Yin , Yin , Yin , Yang}}, // Spades
		{{Yang, Yang, Yang, Yin , Yang, Yang}}, // Hearts
		{{Yang, Yang, Yang, Yin , Yang, Yin }}, // Diamonds
		{{Yin , Yang, Yang, Yang, Yin , Yang}}, // Clubs
	}};
	auto resultHexagrams1 = CalculateHexagrams(info1);
	auto resultHexagrams2 = CalculateHexagrams(info2);
	if (!Compare(etalonHexagrams1, resultHexagrams1) || !Compare(etalonHexagrams2, resultHexagrams2)){
		log(logxx::error) << "Failed" << logxx::endl;
		return false;
	} else
		return true;
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
	ArrayType balancedDeck {{
		{Spades, Jack},	
		{Spades, Nine},	
		{Clubs, Ten},	
		{Hearts, Queen},	
		{Diamonds, Six},	
		{Clubs, Nine},	
		{Clubs, Six},	
		{Hearts, Jack},	
		{Spades, Six},	
		{Spades, Queen},	
		{Diamonds, Jack},	
		{Clubs, Queen},	
		{Clubs, Ace},	
		{Hearts, Seven},	
		{Diamonds, King},	
		{Clubs, Jack},	
		{Diamonds, Queen},	
		{Clubs, Eight},	
		{Spades, Seven},	
		{Spades, Eight},	
		{Hearts, Ace},	
		{Spades, Ace},	
		{Hearts, Six},	
		{Spades, Ten},	
		{Spades, King},	
		{Clubs, Seven},	
		{Diamonds, Seven},	
		{Clubs, King},	
		{Diamonds, Ace},	
		{Hearts, Nine},	
		{Hearts, Ten},	
		{Diamonds, Eight},	
		{Diamonds, Nine},	
		{Hearts, Eight},	
		{Diamonds, Ten},	
		{Hearts, King}
	}};
	Hexagram etalonHexagram {{Yin, Yang, Yang, Yang, Yang, Yang}};

	return TestBalanceAndSuit(balancedDeck, Card::Suit(Hearts), etalonHexagram);
}

bool TestIChing::TestBalanceAndSuit(const ArrayType& deck, const Card::Suit& suit, const i_ching::Hexagram& etalonHexagram){
	S_LOG("TestBalanceAndSuit");
	Patience::PatienceInfo info;
	if (!Patience::Converge(deck, info)){
		log(logxx::error) << "Deck doesn't converge!\n" << deck << logxx::endl;
		return false;
	} else {
		BalanceAndSuitChecker checker(suit, etalonHexagram);
		bool checkResult = checker(deck, info);
		if (!checkResult){
			log(logxx::error) << "Deck doesn't match, but it should" <<
			"\nDeck: " << deck << logxx::endl;
			return false;
		} else
			return true;
	}
}

