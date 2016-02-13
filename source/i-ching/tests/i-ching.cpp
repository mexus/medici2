#include <gtest/gtest.h>

#include <i-ching/i-ching.h>

using namespace standard_36_deck;
using namespace i_ching;

namespace {
static const SuitsHexagrams balanced_hexagrams1{{
    {{Yang, Yang, Yin, Yang, Yin, Yin}},   // Spades
    {{Yin, Yang, Yang, Yin, Yang, Yang}},  // Hearts
    {{Yang, Yin, Yang, Yin, Yang, Yin}},   // Diamonds
    {{Yin, Yin, Yin, Yang, Yin, Yang}},    // Clubs
}};
static const SuitsHexagrams balanced_hexagrams2{{
    {{Yang, Yin, Yang, Yin, Yang, Yin}},  // Spades
    {{Yin, Yang, Yin, Yang, Yin, Yang}},  // Hearts
    {{Yang, Yin, Yang, Yin, Yang, Yin}},  // Diamonds
    {{Yin, Yang, Yin, Yang, Yin, Yang}},  // Clubs
}};
static const SuitsHexagrams unbalanced_hexagrams{{
    {{Yang, Yang, Yin, Yin, Yin, Yang}},    // Spades
    {{Yang, Yang, Yang, Yin, Yang, Yang}},  // Hearts
    {{Yang, Yang, Yang, Yin, Yang, Yin}},   // Diamonds
    {{Yin, Yang, Yang, Yang, Yin, Yang}},   // Clubs
}};
}

TEST(IChing, CalculateCase1) {
    medici::PatienceInfo info;
    info.stationars = {
        {Spades, Jack},   {Spades, Eight},   {Hearts, Six},     {Hearts, Nine},
        {Hearts, Ten},    {Hearts, Jack},    {Hearts, Queen},   {Hearts, King},
        {Diamonds, Six},  {Diamonds, Seven}, {Diamonds, Eight}, {Diamonds, Nine},
        {Diamonds, Jack}, {Diamonds, Ace},   {Clubs, Seven},    {Clubs, Jack}};
    SuitsHexagrams etalon_hexagrams{{
        {{Yang, Yang, Yang, Yang, Yang, Yang}},  // Spades
        {{Yin, Yin, Yin, Yin, Yin, Yang}},       // Hearts
        {{Yin, Yin, Yang, Yang, Yang, Yin}},     // Diamonds
        {{Yang, Yang, Yang, Yang, Yang, Yang}}   // Clubs
    }};

    ASSERT_EQ(CalculateHexagrams(info), etalon_hexagrams);
}

TEST(IChing, CalculateCase2) {
    medici::PatienceInfo info;
    info.stationars = {{Spades, Ten},    {Spades, Jack},    {Spades, Queen},
                       {Spades, King},   {Hearts, Jack},    {Hearts, Queen},
                       {Diamonds, Jack}, {Diamonds, Queen}, {Diamonds, Ace},
                       {Clubs, Nine},    {Clubs, Jack},     {Clubs, King}};
    SuitsHexagrams etalon_hexagrams{{
        {{Yang, Yang, Yin, Yin, Yin, Yang}},    // Spades
        {{Yang, Yang, Yang, Yin, Yang, Yang}},  // Hearts
        {{Yang, Yang, Yang, Yin, Yang, Yin}},   // Diamonds
        {{Yin, Yang, Yang, Yang, Yin, Yang}},   // Clubs
    }};

    ASSERT_EQ(CalculateHexagrams(info), etalon_hexagrams);
}

TEST(IChing, BalanceChecker) {
    BalanceChecker checker;
    ASSERT_TRUE(checker.Check(balanced_hexagrams1));
    ASSERT_TRUE(checker.Check(balanced_hexagrams2));
    ASSERT_FALSE(checker.Check(unbalanced_hexagrams));
}

TEST(IChing, BalanceAndSuitCheckerCorrect) {
    std::set<uint_fast8_t> suits{Spades, Hearts, Diamonds, Clubs};
    for (auto suit : suits) {
        BalanceAndSuitChecker checker1(suit, balanced_hexagrams1[suit]);
        ASSERT_TRUE(checker1.Check(balanced_hexagrams1)) << "on suit " << (int)suit;
        BalanceAndSuitChecker checker2(suit, balanced_hexagrams2[suit]);
        ASSERT_TRUE(checker2.Check(balanced_hexagrams2)) << "on suit " << (int)suit;
    }
}

TEST(IChing, BalanceAndSuitCheckerIncorrect) {
    BalanceAndSuitChecker checker1(0, {});
    ASSERT_FALSE(checker1.Check(balanced_hexagrams1));
}

TEST(IChing, BalanceAndSuitCheckerNonbalanced) {
    BalanceAndSuitChecker checker1(Diamonds, unbalanced_hexagrams[Diamonds]);
    ASSERT_FALSE(checker1.Check(unbalanced_hexagrams));
}
