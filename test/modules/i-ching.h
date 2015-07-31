#ifndef TEST_I_CHING_H
#define TEST_I_CHING_H

#include <i-ching/i-ching.h>
#include <test_fw.h>

class TestIChing : public TestFW {
public:
    TestIChing();

protected:
    static logxx::Log cLog;
    bool Tests();

    static i_ching::SuitsHexagrams CalculateHexagrams(const std::vector<Card>& deck);
    bool TestCalculation();
    bool TestBalance();
    bool TestBalance(const std::vector<Card>& deck, bool balancedEtalon);

    bool TestBalanceAndSuit();
    bool TestBalanceAndSuit(const std::vector<Card>& deck, std::uint_fast8_t suit,
                            const i_ching::Hexagram& etalonHexagram);
};

#endif /* TEST_I_CHING_H */
