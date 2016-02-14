#include <gtest/gtest.h>

#include <cards/simple-sequence.h>

using namespace cards;

TEST(Cards_SimpleSequence, Equals) {
    const SimpleSequence original{{{2, 0}, {1, 4}}, 3};
    const SimpleSequence not_equal1{{{2, 0}, {1, 4}, {1, 5}}, 3};
    const SimpleSequence not_equal2{{{2, 0}, {1, 4}}, 2};

    ASSERT_FALSE(original == not_equal1);
    ASSERT_FALSE(original == not_equal2);
    ASSERT_FALSE(not_equal1 == original);
    ASSERT_FALSE(not_equal2 == original);
    ASSERT_EQ(original, original);
}

TEST(Cards_SimpleSequence, Contradicts) {
    const SimpleSequence empty{};
    const SimpleSequence original{{{2, 0}, {1, 4}}, 3};

    const SimpleSequence contradictory_left{{{2, 1}, {2, 2}}, 2};
    const SimpleSequence contradictory_middle{{{2, 2}}, 4};
    const SimpleSequence contradictory_right{{{1, 3}}, 4};

    const SimpleSequence non_contradictory_left{{{2, 1}, {2, 0}}, 2};
    const SimpleSequence non_contradictory_middle{{{2, 0}}, 3};
    const SimpleSequence non_contradictory_right{{{1, 4}, {2, 1}}, 4};

    const SimpleSequence non_intersecting_left{{{1, 4}, {3, 6}, {2, 0}}, 0};
    const SimpleSequence non_intersecting_right{{{1, 4}, {3, 6}, {2, 0}}, 5};

    ASSERT_FALSE(original.Contradicts(original));
    ASSERT_FALSE(original.Contradicts(empty));

    ASSERT_TRUE(original.Contradicts(contradictory_left));
    ASSERT_TRUE(original.Contradicts(contradictory_middle));
    ASSERT_TRUE(original.Contradicts(contradictory_right));

    ASSERT_FALSE(original.Contradicts(non_contradictory_left));
    ASSERT_FALSE(original.Contradicts(non_contradictory_middle));
    ASSERT_FALSE(original.Contradicts(non_contradictory_right));

    ASSERT_FALSE(original.Contradicts(non_intersecting_left));
    ASSERT_FALSE(original.Contradicts(non_intersecting_right));

    ASSERT_FALSE(empty.Contradicts(empty));
    ASSERT_FALSE(empty.Contradicts(original));
    ASSERT_FALSE(empty.Contradicts(contradictory_right));
    ASSERT_FALSE(empty.Contradicts(contradictory_left));
}

TEST(Cards_SimpleSequence, IsOccupied) {
    const SimpleSequence sequence{{{12, 8}, {7, 62}, {14, 1}, {34, 11}, {75, 116}}, 16};
    for (size_t i = 0; i != 99; ++i) {
        bool is_occupied = i >= 16 && i <= 20;
        ASSERT_EQ(is_occupied, sequence.IsOccupied(i)) << "on i = " << i;
    }
}

TEST(Cards_SimpleSequence, GetCardAssert) {
    const SimpleSequence sequence{{{0, 0}, {0, 1}, {1, 14}}, 1};
    ASSERT_DEBUG_DEATH(sequence.GetCard(0), "not occupied");
}

TEST(Cards_SimpleSequence, GetCard) {
    const SimpleSequence sequence{{{0, 0}, {0, 1}, {1, 14}}, 1};

    const size_t start = sequence.position;
    const size_t end = sequence.position + sequence.cards.size();
    for (size_t position = start; position != end; ++position) {
        ASSERT_EQ(sequence.cards[position - start], sequence.GetCard(position))
            << "on position = " << position;
    }
}

TEST(Cards_SimpleSequence, Overlaps) {
    const SimpleSequence original_sequence{{{0, 0}, {0, 1}, {1, 14}}, 3};

    const SimpleSequence left_not_overlaps{{{0, 0}, {0, 1}}, 0};
    const SimpleSequence left_touches{{{0, 0}, {0, 1}}, 1};
    const SimpleSequence left_overlaps{{{0, 0}, {0, 1}}, 2};

    const SimpleSequence right_not_overlaps{{{0, 0}}, 7};
    const SimpleSequence right_touches{{{0, 0}}, 6};
    const SimpleSequence right_overlaps{{{0, 0}, {0, 1}}, 5};

    ASSERT_TRUE(original_sequence.Overlaps(original_sequence));

    ASSERT_FALSE(original_sequence.Overlaps(left_not_overlaps));
    ASSERT_TRUE(original_sequence.Overlaps(left_touches));
    ASSERT_TRUE(original_sequence.Overlaps(left_overlaps));

    ASSERT_FALSE(original_sequence.Overlaps(right_not_overlaps));
    ASSERT_TRUE(original_sequence.Overlaps(right_touches));
    ASSERT_TRUE(original_sequence.Overlaps(right_overlaps));
}

TEST(Cards_SimpleSequence, MergeAssertNotOverlap) {
    const SimpleSequence original_sequence{{{0, 0}, {0, 1}, {1, 14}}, 3};
    const SimpleSequence not_overlaps{{{0, 0}, {0, 1}}, 0};
    ASSERT_DEBUG_DEATH(original_sequence.Merge(not_overlaps), "not overlap");
}

TEST(Cards_SimpleSequence, MergeAssertContradicts) {
    const SimpleSequence original_sequence{{{0, 0}, {0, 1}, {1, 14}}, 3};
    const SimpleSequence contradicts{{{0, 0}, {0, 1}}, 2};
    ASSERT_DEBUG_DEATH(original_sequence.Merge(contradicts), "contradiction");
}

TEST(Cards_SimpleSequence, Merge) {
    const SimpleSequence original_sequence{{{0, 0}, {0, 1}}, 3};
    ASSERT_EQ(original_sequence, original_sequence.Merge(original_sequence));

    const SimpleSequence left_touches{{{2, 3}, {4, 15}}, 1};
    ASSERT_EQ(SimpleSequence({{{2, 3}, {4, 15}, {0, 0}, {0, 1}}, 1}), original_sequence.Merge(left_touches));

    const SimpleSequence left_overlaps{{{2, 2}, {0, 0}}, 2};
    ASSERT_EQ(SimpleSequence({{{2, 2}, {0, 0}, {0, 1}}, 2}), original_sequence.Merge(left_overlaps));

    const SimpleSequence right_touches{{{2, 0}, {14, 99}}, 5};
    ASSERT_EQ(SimpleSequence({{{0, 0}, {0, 1}, {2, 0}, {14, 99}}, 3}), original_sequence.Merge(right_touches));

    const SimpleSequence right_overlaps{{{0, 1}, {2, 1}}, 4};
    ASSERT_EQ(SimpleSequence({{{0, 0}, {0, 1}, {2, 1}}, 3}), original_sequence.Merge(right_overlaps));
}
