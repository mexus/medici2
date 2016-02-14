#include <gtest/gtest.h>

#include <cards/card.h>

#include "print.h"

using namespace cards;

TEST(Cards, CardLesser) {
    Card card1{0, 0};
    Card card2{0, 1};
    Card card3{1, 0};
    Card card4{1, 1};

    ASSERT_FALSE(card1 < card1);
    ASSERT_TRUE(card1 < card2);
    ASSERT_TRUE(card1 < card3);
    ASSERT_TRUE(card1 < card4);

    ASSERT_FALSE(card2 < card1);
    ASSERT_FALSE(card2 < card2);
    ASSERT_TRUE(card2 < card3);
    ASSERT_TRUE(card2 < card4);

    ASSERT_FALSE(card3 < card1);
    ASSERT_FALSE(card3 < card2);
    ASSERT_FALSE(card3 < card3);
    ASSERT_TRUE(card3 < card4);

    ASSERT_FALSE(card4 < card1);
    ASSERT_FALSE(card4 < card2);
    ASSERT_FALSE(card4 < card3);
    ASSERT_FALSE(card4 < card4);
}

TEST(Cards, CardEqual) {
    Card card1{0, 0};
    Card card2{0, 1};
    Card card3{0, 0};

    ASSERT_TRUE(card1 == card1);
    ASSERT_FALSE(card1 == card2);
    ASSERT_TRUE(card1 == card3);

    ASSERT_FALSE(card2 == card1);
    ASSERT_TRUE(card2 == card2);
    ASSERT_FALSE(card2 == card3);

    ASSERT_TRUE(card3 == card1);
    ASSERT_FALSE(card3 == card2);
    ASSERT_TRUE(card3 == card3);
}

TEST(Cards, CardNotEqual) {
    Card card1{0, 0};
    Card card2{0, 1};
    Card card3{0, 0};

    ASSERT_FALSE(card1 != card1);
    ASSERT_TRUE(card1 != card2);
    ASSERT_FALSE(card1 != card3);

    ASSERT_TRUE(card2 != card1);
    ASSERT_FALSE(card2 != card2);
    ASSERT_TRUE(card2 != card3);

    ASSERT_FALSE(card3 != card1);
    ASSERT_TRUE(card3 != card2);
    ASSERT_FALSE(card3 != card3);
}
