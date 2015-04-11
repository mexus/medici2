#ifndef TEST_I_CHING_H
#define TEST_I_CHING_H

#include <i-ching/i-ching.h>
#include <test_fw.h>

class TestIChing : public TestFW{
public:
    typedef standard_36_deck::Deck DeckType;
    typedef standard_36_deck::Deck::ArrayType ArrayType;

    TestIChing();
protected:
    static logxx::Log cLog;
    bool Tests();

    static i_ching::SuitsHexagrams CalculateHexagrams(const ArrayType& deck);
    bool TestCalculation();
    bool TestBalance();
    bool TestBalance(const ArrayType& deck, bool balancedEtalon);

    bool TestBalanceAndSuit();
    bool TestBalanceAndSuit(const ArrayType& deck, const Card::Suit&, const i_ching::Hexagram& etalonHexagram);
};

#endif /* TEST_I_CHING_H */
