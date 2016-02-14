#include <gtest/gtest.h>

#include <cards/any-rank-condition.h>

using namespace cards;

namespace {

::std::ostream& operator<<(::std::ostream& s, const Sequence& sequence) {
    for (auto& part : sequence.GetParts()) {
        s << "<At " << part.position << ":";
        for (auto& card : part.cards) {
            s << " [" << (int)card.suit << ", " << (int)card.rank << "]";
        }
        s << "> ";
    }
    return s;
}
}

TEST(Cards_AnyRankCondition, Check) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {1, 2}};

    ASSERT_TRUE(AnyRankCondition(0, 0).CheckSequence(deck));
    ASSERT_TRUE(AnyRankCondition(1, 0).CheckSequence(deck));
    ASSERT_FALSE(AnyRankCondition(2, 0).CheckSequence(deck));

    ASSERT_FALSE(AnyRankCondition(0, 0, true).CheckSequence(deck));
    ASSERT_FALSE(AnyRankCondition(1, 0, true).CheckSequence(deck));
    ASSERT_TRUE(AnyRankCondition(2, 0, true).CheckSequence(deck));
}

namespace {
using Sequences = std::vector<Sequence>;
}

TEST(Cards_AnyRankCondition, GetVariantsOverlaps) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {1, 2}}, 0});
    Storage empty_storage({});

    ASSERT_EQ(Sequences({SimpleSequence{{{0, 0}}, 0}}),
              AnyRankCondition(0, 0).GetVariants(sequence, empty_storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{0, 1}}, 1}}),
              AnyRankCondition(1, 0).GetVariants(sequence, empty_storage));
    ASSERT_TRUE(AnyRankCondition(2, 0).GetVariants(sequence, empty_storage).empty());

    ASSERT_TRUE(
        AnyRankCondition(0, 0, true).GetVariants(sequence, empty_storage).empty());
    ASSERT_TRUE(
        AnyRankCondition(1, 0, true).GetVariants(sequence, empty_storage).empty());
    ASSERT_EQ(Sequences({SimpleSequence{{{1, 2}}, 2}}),
              AnyRankCondition(2, 0, true).GetVariants(sequence, empty_storage));
}

TEST(Cards_AnyRankCondition, GetVariants) {
    Storage storage({{0, 2}, {0, 3}, {0, 4}, {1, 3}});

    ASSERT_EQ(Sequences({SimpleSequence{{{0, 2}}, 0}, SimpleSequence{{{0, 3}}, 0},
                         SimpleSequence{{{0, 4}}, 0}}),
              AnyRankCondition(0, 0).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{1, 3}}, 0}}),
              AnyRankCondition(0, 1).GetVariants({}, storage));
    ASSERT_TRUE(AnyRankCondition(0, 2).GetVariants({}, storage).empty());

    ASSERT_EQ(Sequences({SimpleSequence{{{1, 3}}, 0}}),
              AnyRankCondition(0, 0, true).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{0, 2}}, 0}, SimpleSequence{{{0, 3}}, 0},
                         SimpleSequence{{{0, 4}}, 0}}),
              AnyRankCondition(0, 1, true).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{0, 2}}, 0}, SimpleSequence{{{0, 3}}, 0},
                         SimpleSequence{{{1, 3}}, 0}, SimpleSequence{{{0, 4}}, 0}}),
              AnyRankCondition(0, 2, true).GetVariants({}, storage));
}
