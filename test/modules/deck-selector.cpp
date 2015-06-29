#include "deck-selector.h"
#include <operators.h>
#include "operators.h"

using namespace standard_36_deck;
logxx::Log TestDeckSelector::cLog("TestDeckSelector");
CardSelectorConfigurator TestDeckSelector::configurator;

TestDeckSelector::TestDeckSelector() :
    TestFW("deck-selector")
{
}

bool TestDeckSelector::Tests()
{
    return TestAllSelector() && TestOneSelector() && TestComplex();
}

CardSelector TestDeckSelector::SelectorAnyRank(std::uint_fast8_t suit, bool straight)
{
    configurator.Reset();
    configurator.SetSuit(suit);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}

CardSelector TestDeckSelector::SelectorAnySuit(std::uint_fast8_t rank, bool straight)
{
    configurator.Reset();
    configurator.SetRank(rank);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}

CardSelector TestDeckSelector::Selector(std::uint_fast8_t suit, std::uint_fast8_t rank, bool straight)
{
    configurator.Reset();
    configurator.SetSuit(suit);
    configurator.SetRank(rank);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}

bool TestDeckSelector::TestAllSelector()
{
    std::vector<Card> deck1 {
        {Diamonds, Nine},
        {Hearts, Nine},
        {Clubs, Jack}
    };
    //All should be "Nine"s and "Non-Spades"s in a range {0, 1}:
    DeckAllSelector deckSelector1({SelectorAnySuit(Nine), SelectorAnyRank(Spades, false)}, 0, 1);
    //All should be "Not-Eight"s in a range {0, 1}:
    DeckAllSelector deckSelector2({SelectorAnySuit(Eight, false)}, 0, 1);
    //All should be "Nine"s in a range {0, 2}:
    DeckAllSelector deckSelector3({SelectorAnySuit(Nine)}, 0, 2);

    return TestSelector(deck1, deckSelector1, true) && TestSelector(deck1, deckSelector2, true) && TestSelector(deck1, deckSelector3, false);
}

bool TestDeckSelector::TestOneSelector()
{
    std::vector<Card> deck1 {
        {Spades, King},
        {Hearts, Queen},
        {Clubs, Jack},
    };
    //Should be at least one "King-of-Spades" in a range {0, 1}
    DeckOneSelector deckSelector1({Selector(Spades, King)}, 0, 1);
    //Should be at least one "King-of-Spades" or "Queen-of-Hearts" in a range {1, 2}
    DeckOneSelector deckSelector2({Selector(Spades, King), Selector(Hearts, Queen)}, 0, 1);
    //Should not be at least one "King-of-Spades" or "Not-Ace" in a range {2, 2}
    DeckOneSelector deckSelector3({Selector(Spades, King), SelectorAnySuit(Ace, false)}, 2, 2);
    //Should not be at least one "King-of-Spades" or "Not-Jack" in a range {2, 2}
    DeckOneSelector deckSelector4({Selector(Spades, King), SelectorAnySuit(Jack, false)}, 2, 2);

    return TestSelector(deck1, deckSelector1, true) && TestSelector(deck1, deckSelector2, true) && TestSelector(deck1, deckSelector3, true) && 
        TestSelector(deck1, deckSelector4, false);
}

bool TestDeckSelector::TestComplex()
{
    S_LOG("TestComplex");
    using namespace standard_36_deck;
    std::vector<Card> deck1 {
        {Hearts, Jack},     // 0
        {Clubs, Nine},      // 1
        {Hearts, Ace},      // 2
        {Clubs, Jack},      // 3
        {Spades, Seven},    // 4
        {Clubs, Ten},       // 5
        {Spades, Nine},     // 6
        {Hearts, Seven},    // 7
        {Hearts, Nine},     // 8
        {Diamonds, Ace},    // 9
        {Hearts, Six},      // 10
        {Spades, Queen},    // 11
        {Hearts, Eight},    // 12
        {Diamonds, King},   // 13
        {Spades, Ace},      // 14
        {Spades, Ten},      // 15
        {Diamonds, Jack},   // 16
        {Spades, Eight},    // 17
        {Clubs, Seven},     // 18
        {Diamonds, Nine},   // 19
        {Spades, Six},      // 20
        {Hearts, Queen},    // 21
        {Hearts, King},     // 22 
        {Spades, Jack},     // 23
        {Hearts, Ten},      // 24
        {Spades, King},     // 25
        {Clubs, Eight},     // 26
        {Diamonds, Queen},  // 27
        {Diamonds, Ten},    // 28
        {Diamonds, Eight},  // 29
        {Clubs, Six},       // 30
        {Clubs, Queen},     // 31
        {Diamonds, Seven},  // 32
        {Clubs, Ace},       // 33
        {Diamonds, Six},    // 34
        {Clubs, King}       // 35
    };

    DeckOneSelector targetCard({Selector(Hearts, Ten)}, 19, 24);
    DeckAllSelector ownActions({SelectorAnySuit(Ace, false)}, 3, 7);
    DeckOneSelector firstCard({SelectorAnySuit(Jack)}, 0, 0);
    DeckOneSelector secondCard({SelectorAnySuit(Nine)}, 1, 1);
    DeckOneSelector thirdCard({SelectorAnySuit(Ace), SelectorAnySuit(Ten)}, 2, 2);

    DeckSelectors selectors;
    selectors.AddDeckSelector(targetCard);
    selectors.AddDeckSelector(ownActions);
    selectors.AddDeckSelector(firstCard);
    selectors.AddDeckSelector(secondCard);
    selectors.AddDeckSelector(thirdCard);

    if (!selectors.Check(deck1)) {
        log(logxx::error) << "Failed" << logxx::endl;
        return false;
    } else
        return true;
}

void PrintDeckSelectorInfo(std::ostream& s, const DeckAbstractSelector& abstractSelector)
{
    s << "range {" << abstractSelector.from << ", " << abstractSelector.to << "}, ";
    s << "card selectors: {" << abstractSelector.cardSelectors << "}";
}

std::ostream& operator <<(std::ostream& s, const DeckAbstractSelector& abstractSelector)
{
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

void PrintDeckPart(std::ostream& s, const std::vector<Card>& deck, std::size_t from, std::size_t to)
{
    std::vector<Card> cards(deck.begin() + from, deck.begin() + to + 1);
    s << cards;
}

bool TestDeckSelector::TestSelector(const std::vector<Card>& deck, const DeckAbstractSelector& deckSelector, bool etalonResult)
{
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

