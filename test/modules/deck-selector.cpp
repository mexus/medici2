#include "deck-selector.h"
#include "operators.h"

using namespace standard_36_deck;
logxx::Log TestDeckSelector::cLog("TestDeckSelector");

TestDeckSelector::TestDeckSelector() : TestFW("deck-selector"){
}

bool TestDeckSelector::Tests(){
	return TestNegateSelector() && TestAllSelector() && TestOneSelector();
}

bool TestDeckSelector::TestNegateSelector(){
	auto deck1 = DeckType::cards;
	deck1[0] = {Hearts, Six};
	deck1[1] = {Hearts, Ten};
	deck1[2] = {Clubs, Ace};
	//No "Heart"s in {0, 1}, e.g. all should be non-hearts:
	DeckNegateSelector deckSelector1({CardSelector(Card::Suit(Hearts), true)}, 0, 1); 
	//No "No-Heart"s in {0, 1}, e.g. all should be hearts:
	DeckNegateSelector deckSelector2({CardSelector(Card::Suit(Hearts), false)}, 0, 1);
	//Same as previous, but range is {0, 2}
	DeckNegateSelector deckSelector3({CardSelector(Card::Suit(Hearts), false)}, 0, 2);

	return TestSelector(deck1, deckSelector1, false) && TestSelector(deck1, deckSelector2, true) && TestSelector(deck1, deckSelector3, false);
}

bool TestDeckSelector::TestAllSelector(){
	auto deck1 = DeckType::cards;
	deck1[0] = {Diamonds, Nine};
	deck1[1] = {Hearts, Nine};
	deck1[2] = {Clubs, Jack};
	//All should be "Nine"s and "Non-Spades"s in a range {0, 1}:
	DeckAllSelector deckSelector1({CardSelector(Card::Rank(Nine), true), CardSelector(Card::Suit(Spades), false)}, 0, 1);
	//All should be "Not-Eight"s in a range {0, 1}:
	DeckAllSelector deckSelector2({CardSelector(Card::Rank(Eight), false)}, 0, 1);
	//All should be "Nine"s in a range {0, 2}:
	DeckAllSelector deckSelector3({CardSelector(Card::Rank(Nine), true)}, 0, 2);

	return TestSelector(deck1, deckSelector1, true) && TestSelector(deck1, deckSelector2, true) && TestSelector(deck1, deckSelector3, false);
}

bool TestDeckSelector::TestOneSelector(){
	auto deck1 = DeckType::cards;
	deck1[0] = {Spades, King};
	deck1[1] = {Hearts, Queen};
	deck1[2] = {Clubs, Jack};
	//Should be at least one "King-of-Spades" in a range {0, 1}
	DeckOneSelector deckSelector1({CardSelector(Card::Suit(Spades), Card::Rank(King), true)}, 0, 1);
	//Should be at least one "King-of-Spades" or "Queen-of-Hearts" in a range {1, 2}
	DeckOneSelector deckSelector2({CardSelector(Card::Suit(Spades), Card::Rank(King), true), CardSelector(Card::Suit(Hearts), Card::Rank(Queen), true)}, 0, 1);
	//Should not be at least one "King-of-Spades" or "Not-Ace" in a range {2, 2}
	DeckOneSelector deckSelector3({CardSelector(Card::Suit(Spades), Card::Rank(King), true), CardSelector(Card::Rank(Ace), false)}, 2, 2);
	//Should not be at least one "King-of-Spades" or "Not-Jack" in a range {2, 2}
	DeckOneSelector deckSelector4({CardSelector(Card::Suit(Spades), Card::Rank(King), true), CardSelector(Card::Rank(Jack), false)}, 2, 2);

	return TestSelector(deck1, deckSelector1, true) && TestSelector(deck1, deckSelector2, true) && TestSelector(deck1, deckSelector3, true) && 
		TestSelector(deck1, deckSelector4, false);
}

void PrintDeckSelectorInfo(std::ostream& s, const DeckAbstractSelector& abstractSelector){
	s << "range {" << abstractSelector.from << ", " << abstractSelector.to << "}, ";
	s << "card selectors: {" << abstractSelector.cardSelectors << "}";
}

std::ostream& operator <<(std::ostream& s, const DeckAbstractSelector& abstractSelector){
	try {
		auto &selector = dynamic_cast<const DeckNegateSelector&>(abstractSelector);
		s << "DeckNegateSelector ";
		PrintDeckSelectorInfo(s, selector);
		return s;
	} catch (const std::bad_cast&) {}
	try {
		auto &selector = dynamic_cast<const DeckAllSelector&>(abstractSelector);
		s << "DeckAllSelector ";
		PrintDeckSelectorInfo(s, selector);
		return s;
	} catch (const std::bad_cast&) {}
	try {
		auto &selector = dynamic_cast<const DeckOneSelector&>(abstractSelector);
		s << "DeckOneSelector ";
		PrintDeckSelectorInfo(s, selector);
		return s;
	} catch (const std::bad_cast&) {}
	s << "Unknown deck selector ";
	PrintDeckSelectorInfo(s, abstractSelector);

	return s;
}

void PrintDeckPart(std::ostream& s, const TestDeckSelector::ArrayType& deck, std::size_t from, std::size_t to){
	std::vector<Card> cards(deck.begin() + from, deck.begin() + to + 1);
	s << cards;
}

bool TestDeckSelector::TestSelector(const ArrayType& deck, const DeckAbstractSelector& deckSelector, bool etalonResult){
	S_LOG("TestSelector");
	auto result = deckSelector.Check(deck);
	if (result == etalonResult)
		return true;
	else{
		auto &s = log(logxx::error) << "\nDeck: ";
		PrintDeckPart(s, deck, deckSelector.from, deckSelector.to);
		s << "\n" << "Deck selector: " << deckSelector << "\n" << "Should return " << 
			std::boolalpha << etalonResult << ", but returned " << std::boolalpha << result << logxx::endl;
		return false;
	}
}

