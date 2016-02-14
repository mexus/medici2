#include <gtest/gtest.h>

#include <cards/one-card-condition.h>

#include "print.h"

using namespace cards;

TEST(Cards_OneCardCondition, Check) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {0, 2}};

    ASSERT_TRUE(OneCardCondition(0, Card{0, 0}).CheckSequence(deck));
    ASSERT_FALSE(OneCardCondition(0, Card{1, 0}).CheckSequence(deck));
    ASSERT_TRUE(OneCardCondition(1, Card{0, 1}).CheckSequence(deck));
    ASSERT_TRUE(OneCardCondition(2, Card{0, 2}).CheckSequence(deck));
}

TEST(Cards_OneCardCondition, CheckInverse) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {0, 2}};

    ASSERT_FALSE(OneCardCondition(0, Card{0, 0}, true).CheckSequence(deck));
    ASSERT_TRUE(OneCardCondition(0, Card{1, 0}, true).CheckSequence(deck));
    ASSERT_FALSE(OneCardCondition(1, Card{0, 1}, true).CheckSequence(deck));
    ASSERT_FALSE(OneCardCondition(2, Card{0, 2}, true).CheckSequence(deck));
}

TEST(Cards_OneCardCondition, GetVariantsOverlaps) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});

    auto variants = OneCardCondition(1, Card{0, 1}).GetVariants(sequence, Storage({}));

    ASSERT_EQ(1, variants.size());
    ASSERT_EQ(1, variants[0].GetParts().size());
    ASSERT_EQ(1, variants[0].GetParts()[0].position);
    ASSERT_EQ(std::vector<Card>({{0, 1}}), variants[0].GetParts()[0].cards);
}

TEST(Cards_OneCardCondition, GetVariantsInverseOverlaps) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});

    auto variants =
        OneCardCondition(1, Card{0, 0}, true).GetVariants(sequence, Storage({}));

    ASSERT_EQ(1, variants.size());
    ASSERT_EQ(1, variants[0].GetParts().size());
    ASSERT_EQ(1, variants[0].GetParts()[0].position);
    ASSERT_EQ(std::vector<Card>({{0, 1}}), variants[0].GetParts()[0].cards);
}

TEST(Cards_OneCardCondition, GetVariantsOverlapsContradicts) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});

    auto variants = OneCardCondition(1, Card{0, 2}).GetVariants(sequence, Storage({}));

    ASSERT_TRUE(variants.empty());
}

TEST(Cards_OneCardCondition, GetVariantsInverseOverlapsContradicts) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});

    auto variants =
        OneCardCondition(1, Card{0, 1}, true).GetVariants(sequence, Storage({}));

    ASSERT_TRUE(variants.empty());
}

TEST(Cards_OneCardCondition, GetVariantsEmptyStorage) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});
    Storage storage({});

    ASSERT_TRUE(OneCardCondition(3, Card{0, 3}).GetVariants(sequence, storage).empty());
}

TEST(Cards_OneCardCondition, GetVariants) {
    const Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {0, 2}}, 0});
    const Storage storage({{0, 3}, {0, 4}});

    auto variants = OneCardCondition(3, Card{0, 3}).GetVariants(sequence, storage);
    ASSERT_EQ(1, variants.size());
    ASSERT_EQ(Sequence(SimpleSequence{{{0, 3}}, 3}), variants[0]);

    variants = OneCardCondition(3, Card{3, 3}, true).GetVariants(sequence, storage);
    ASSERT_EQ(2, variants.size());
    ASSERT_EQ(Sequence(SimpleSequence{{{0, 3}}, 3}), variants[0]);
    ASSERT_EQ(Sequence(SimpleSequence{{{0, 4}}, 3}), variants[1]);
}
