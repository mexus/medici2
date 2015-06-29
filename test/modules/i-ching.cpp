#include "i-ching.h"
#include <operators.h>
#include <comparisons.h>
#include "operators.h"

logxx::Log TestIChing::cLog("TestIChing");
using namespace i_ching;
using namespace medici;
using namespace standard_36_deck;

TestIChing::TestIChing() : TestFW("i-ching")
{
}

bool TestIChing::Tests()
{
    return TestCalculation() && TestBalance() && TestBalanceAndSuit();
}

SuitsHexagrams TestIChing::CalculateHexagrams(const std::vector<Card>& deck)
{
    PatienceInfo info;
    if (TryToConverge(deck, info)) {
        return i_ching::CalculateHexagrams(info);
    } else
        throw std::logic_error("Supplied not convergable deck");
}

bool TestIChing::TestCalculation()
{
    S_LOG("TestCalculation");
    PatienceInfo info1, info2;
    info1.stationars = {{Spades, Jack}, {Spades, Eight}, {Hearts, Six}, {Hearts, Nine}, {Hearts, Ten}, {Hearts, Jack}, {Hearts, Queen}, {Hearts, King}, {Diamonds, Six}, {Diamonds, Seven}, {Diamonds, Eight}, {Diamonds, Nine}, {Diamonds, Jack}, {Diamonds, Ace}, {Clubs, Seven}, {Clubs, Jack}};
    info2.stationars = {{Spades, Ten}, {Spades, Jack}, {Spades, Queen}, {Spades, King}, {Hearts, Jack}, {Hearts, Queen}, {Diamonds, Jack}, {Diamonds, Queen}, {Diamonds, Ace}, {Clubs, Nine}, {Clubs, Jack}, {Clubs, King}};
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
    std::vector<Card> deck3 {{{Spades, Seven}, {Clubs, Eight}, {Clubs, Ace}, {Hearts, Six}, {Hearts, King}, {Diamonds, Nine}, {Clubs, Ten}, {Spades, Nine}, {Hearts, Seven}, {Diamonds, King}, {Clubs, Queen}, {Diamonds, Ten}, {Clubs, Nine}, {Spades, Queen}, {Spades, Ten}, {Diamonds, Seven}, {Spades, Ace}, {Spades, Six}, {Clubs, Jack}, {Hearts, Ten}, {Diamonds, Eight}, {Hearts, Eight}, {Spades, Jack}, {Diamonds, Queen}, {Spades, Eight}, {Diamonds, Six}, {Clubs, Six}, {Diamonds, Jack}, {Hearts, Jack}, {Diamonds, Ace}, {Hearts, Queen}, {Spades, King}, {Hearts, Ace}, {Clubs, King}, {Hearts, Nine}, {Clubs, Seven}}};
    SuitsHexagrams etalonHexagrams3 {{
        {{Yin , Yin , Yin , Yin , Yang, Yin }}, // Spades
        {{Yang, Yang, Yin , Yin , Yin , Yang}}, // Hearts
        {{Yin , Yin , Yang, Yang, Yin , Yang}}, // Diamonds
        {{Yang, Yang, Yang, Yang, Yang, Yin }}, // Clubs
    }};
    auto resultHexagrams1 = i_ching::CalculateHexagrams(info1);
    auto resultHexagrams2 = i_ching::CalculateHexagrams(info2);
    auto resultHexagrams3 = CalculateHexagrams(deck3);
    if (!::Compare(etalonHexagrams1, resultHexagrams1) || !::Compare(etalonHexagrams2, resultHexagrams2) || 
            !::Compare(etalonHexagrams3, resultHexagrams3)) {
        log(logxx::error) << "Failed" << logxx::endl;
        return false;
    } else
        return true;
}

bool TestIChing::TestBalance()
{
    std::vector<Card> balancedDeck {{{Hearts, Jack}, {Hearts, Nine}, {Hearts, Ten}, {Diamonds, Queen}, {Clubs, Seven}, {Hearts, Seven}, {Diamonds, Jack}, {Spades, Jack}, {Hearts, Six}, {Hearts, Queen}, {Diamonds, Seven}, {Spades, Ace}, {Spades, Queen}, {Spades, Ten}, {Clubs, Jack}, {Clubs, Nine}, {Diamonds, Nine}, {Diamonds, Ten}, {Diamonds, Six}, {Clubs, Queen}, {Diamonds, Ace}, {Clubs, Ten}, {Clubs, Six}, {Diamonds, King}, {Hearts, Ace}, {Clubs, Eight}, {Hearts, Eight}, {Spades, Six}, {Hearts, King}, {Clubs, King}, {Spades, Nine}, {Spades, King}, {Diamonds, Eight}, {Spades, Seven}, {Clubs, Ace}, {Spades, Eight}}} ;
    std::vector<Card> unbalancedDeck {{{Spades, Jack}, {Hearts, Nine}, {Diamonds, Ace}, {Clubs, Six}, {Clubs, Queen}, {Spades, Queen}, {Spades, Seven}, {Clubs, Jack}, {Diamonds, Queen}, {Spades, Six}, {Diamonds, King}, {Spades, Ten}, {Diamonds, Ten}, {Hearts, King}, {Clubs, King}, {Hearts, Ten}, {Spades, Nine}, {Diamonds, Eight}, {Clubs, Ten}, {Clubs, Ace}, {Hearts, Six}, {Hearts, Queen}, {Clubs, Eight}, {Hearts, Ace}, {Diamonds, Six}, {Diamonds, Seven}, {Clubs, Nine}, {Hearts, Eight}, {Clubs, Seven}, {Diamonds, Nine}, {Spades, Ace}, {Diamonds, Jack}, {Spades, Eight}, {Hearts, Seven}, {Spades, King}, {Hearts, Jack}}};
    std::vector<Card> balanced2 = {{{Hearts, Jack}, {Clubs, Nine}, {Spades, Ten}, {Hearts, Six}, {Diamonds, Six}, {Spades, Nine}, {Spades, Queen}, {Spades, Six}, {Hearts, Eight}, {Clubs, King}, {Spades, King}, {Clubs, Queen}, {Spades, Eight}, {Hearts, Ten}, {Diamonds, Eight}, {Diamonds, Seven}, {Hearts, Queen}, {Clubs, Eight}, {Diamonds, Nine}, {Spades, Seven}, {Hearts, Ace}, {Spades, Jack}, {Diamonds, Ten}, {Diamonds, King}, {Hearts, Seven}, {Clubs, Ace}, {Spades, Ace}, {Hearts, King}, {Diamonds, Ace}, {Diamonds, Queen}, {Clubs, Ten}, {Diamonds, Jack}, {Clubs, Six}, {Clubs, Jack}, {Hearts, Nine}, {Clubs, Seven}}};
    std::vector<Card> balanced3 = {{{Clubs, Nine}, {Diamonds, Eight}, {Hearts, Ten}, {Spades, Queen}, {Hearts, King}, {Clubs, Eight}, {Spades, Ten}, {Clubs, Ten}, {Clubs, King}, {Diamonds, Nine}, {Diamonds, King}, {Hearts, Seven}, {Hearts, Eight}, {Diamonds, Ten}, {Diamonds, Ace}, {Hearts, Jack}, {Hearts, Ace}, {Spades, King}, {Diamonds, Six}, {Clubs, Ace}, {Diamonds, Queen}, {Spades, Nine}, {Clubs, Queen}, {Diamonds, Jack}, {Hearts, Nine}, {Clubs, Seven}, {Hearts, Queen}, {Diamonds, Seven}, {Clubs, Six}, {Spades, Ace}, {Clubs, Jack}, {Hearts, Six}, {Spades, Eight}, {Spades, Jack}, {Spades, Seven}, {Spades, Six}}};
    return TestBalance(balancedDeck, true) && TestBalance(unbalancedDeck, false) && TestBalance(balanced2, true) && TestBalance(balanced3, true);
}

bool TestIChing::TestBalance(const std::vector<Card>& deck, bool balancedEtalon)
{
    S_LOG("TestBalance");
    PatienceInfo info;
    if (!TryToConverge(deck, info)) {
        log(logxx::error) << "Deck doesn't converge!\n" << deck << logxx::endl;
        return false;
    } else {
        BalanceChecker checker;
        bool balancedResult = checker.Check(info);
        if (balancedResult != balancedEtalon) {
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

bool TestIChing::TestBalanceAndSuit()
{
    std::vector<Card> balancedDeck {{{Spades, Jack}, {Spades, Nine}, {Clubs, Ten}, {Hearts, Queen}, {Diamonds, Six}, {Clubs, Nine}, {Clubs, Six}, {Hearts, Jack}, {Spades, Six}, {Spades, Queen}, {Diamonds, Jack}, {Clubs, Queen}, {Clubs, Ace}, {Hearts, Seven}, {Diamonds, King}, {Clubs, Jack}, {Diamonds, Queen}, {Clubs, Eight}, {Spades, Seven}, {Spades, Eight}, {Hearts, Ace}, {Spades, Ace}, {Hearts, Six}, {Spades, Ten}, {Spades, King}, {Clubs, Seven}, {Diamonds, Seven}, {Clubs, King}, {Diamonds, Ace}, {Hearts, Nine}, {Hearts, Ten}, {Diamonds, Eight}, {Diamonds, Nine}, {Hearts, Eight}, {Diamonds, Ten}, {Hearts, King}
    }};
    Hexagram etalonHexagram {{Yin, Yang, Yang, Yang, Yang, Yang}};

    return TestBalanceAndSuit(balancedDeck, Hearts, etalonHexagram);
}

bool TestIChing::TestBalanceAndSuit(const std::vector<Card>& deck, std::uint_fast8_t suit, const i_ching::Hexagram& etalonHexagram)
{
    S_LOG("TestBalanceAndSuit");
    PatienceInfo info;
    if (!TryToConverge(deck, info)) {
        log(logxx::error) << "Deck doesn't converge!\n" << deck << logxx::endl;
        return false;
    } else {
        BalanceAndSuitChecker checker(suit, etalonHexagram);
        bool checkResult = checker.Check(info);
        if (!checkResult) {
            log(logxx::error) << "Deck doesn't match, but it should" <<
            "\nDeck: " << deck << logxx::endl;
            return false;
        } else
            return true;
    }
}

