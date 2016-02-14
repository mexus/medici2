#include <gtest/gtest.h>

#include <cards/any-suit-condition.h>

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

TEST(Cards_AnySuitCondition, Check) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {1, 2}};

    ASSERT_TRUE(AnySuitCondition(0, 0).CheckSequence(deck));
    ASSERT_TRUE(AnySuitCondition(1, 1).CheckSequence(deck));
    ASSERT_FALSE(AnySuitCondition(2, 0).CheckSequence(deck));

    ASSERT_FALSE(AnySuitCondition(0, 0, true).CheckSequence(deck));
    ASSERT_FALSE(AnySuitCondition(1, 1, true).CheckSequence(deck));
    ASSERT_TRUE(AnySuitCondition(2, 0, true).CheckSequence(deck));
}

namespace {
using Sequences = std::vector<Sequence>;
}

TEST(Cards_AnySuitCondition, GetVariantsOverlaps) {
    Sequence sequence(SimpleSequence{{{0, 0}, {0, 1}, {1, 2}}, 0});
    Storage empty_storage({});

    ASSERT_EQ(Sequences({SimpleSequence{{{0, 0}}, 0}}),
              AnySuitCondition(0, 0).GetVariants(sequence, empty_storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{0, 1}}, 1}}),
              AnySuitCondition(1, 1).GetVariants(sequence, empty_storage));
    ASSERT_TRUE(AnySuitCondition(2, 0).GetVariants(sequence, empty_storage).empty());

    ASSERT_TRUE(
        AnySuitCondition(0, 0, true).GetVariants(sequence, empty_storage).empty());
    ASSERT_TRUE(
        AnySuitCondition(1, 1, true).GetVariants(sequence, empty_storage).empty());
    ASSERT_EQ(Sequences({SimpleSequence{{{1, 2}}, 2}}),
              AnySuitCondition(2, 0, true).GetVariants(sequence, empty_storage));
}

TEST(Cards_AnySuitCondition, GetVariants) {
    Storage storage({{2, 0}, {3, 0}, {4, 0}, {3, 1}});

    ASSERT_EQ(Sequences({SimpleSequence{{{2, 0}}, 0}, SimpleSequence{{{3, 0}}, 0},
                         SimpleSequence{{{4, 0}}, 0}}),
              AnySuitCondition(0, 0).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{3, 1}}, 0}}),
              AnySuitCondition(0, 1).GetVariants({}, storage));
    ASSERT_TRUE(AnySuitCondition(0, 2).GetVariants({}, storage).empty());

    ASSERT_EQ(Sequences({SimpleSequence{{{3, 1}}, 0}}),
              AnySuitCondition(0, 0, true).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{2, 0}}, 0}, SimpleSequence{{{3, 0}}, 0},
                         SimpleSequence{{{4, 0}}, 0}}),
              AnySuitCondition(0, 1, true).GetVariants({}, storage));
    ASSERT_EQ(Sequences({SimpleSequence{{{2, 0}}, 0}, SimpleSequence{{{3, 0}}, 0},
                         SimpleSequence{{{4, 0}}, 0}, SimpleSequence{{{3, 1}}, 0}}),
              AnySuitCondition(0, 2, true).GetVariants({}, storage));
}
