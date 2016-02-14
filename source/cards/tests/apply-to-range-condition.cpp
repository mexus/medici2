#include <gtest/gtest.h>

#include <cards/apply-to-range-condition.h>

using namespace cards;

TEST(Cards_ApplyToRangeCondition, ConstructorAssert) {
    ASSERT_DEBUG_DEATH({ ApplyToRangeCondition condition(12, 11, {}); },
                       "`range_end` should not be less than `range_start`");
}

namespace {

struct FakeCondition : Condition {
    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage /*storage*/) const override {
        auto& parts = applied_sequence.GetParts();
        if (parts.empty()) {
            return {};
        }
        if (parts.size() != 1) {
            throw std::logic_error("Only simple sequences are supported");
        }
        auto& part = parts[0];
        if (part.position != 0) {
            throw std::logic_error(
                "Only simple sequences started from zero are supported");
        }
        checked_decks.push_back(part.cards);
        return {applied_sequence};
    }

    bool CheckSequence(const std::vector<Card>& cards) const override {
        checked_decks.push_back(cards);
        return true;
    }

    mutable std::vector<std::vector<Card>> checked_decks;
};
}

TEST(Cards_ApplyToRangeCondition, Check) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {0, 2}, {0, 3}};

    auto checker = std::make_shared<FakeCondition>();
    ApplyToRangeCondition condition(1, 4, checker);

    condition.CheckSequence(deck);

    ASSERT_EQ(3, checker->checked_decks.size());
    ASSERT_EQ(std::vector<Card>({{0, 1}, {0, 2}, {0, 3}}), checker->checked_decks[0]);
    ASSERT_EQ(std::vector<Card>({{0, 2}, {0, 3}}), checker->checked_decks[1]);
    ASSERT_EQ(std::vector<Card>({{0, 3}}), checker->checked_decks[2]);
}

TEST(Cards_ApplyToRangeCondition, GetVariants) {
    auto checker = std::make_shared<FakeCondition>();
    ApplyToRangeCondition range_condtion(1, 3, checker);

    auto vartiants = range_condtion.GetVariants(
        SimpleSequence{{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, 0}, Storage({}));

    ASSERT_EQ(1, vartiants.size());
    ASSERT_EQ(1, vartiants[0].GetParts().size());
    ASSERT_EQ(0, vartiants[0].GetParts()[0].position);
    ASSERT_EQ(std::vector<Card>({{0, 0}, {0, 1}, {0, 2}, {0, 3}}),
              vartiants[0].GetParts()[0].cards);

    ASSERT_EQ(3, checker->checked_decks.size());
    ASSERT_EQ(std::vector<Card>({{0, 1}, {0, 2}, {0, 3}}), checker->checked_decks[0]);
    ASSERT_EQ(std::vector<Card>({{0, 2}, {0, 3}}), checker->checked_decks[1]);
    ASSERT_EQ(std::vector<Card>({{0, 3}}), checker->checked_decks[2]);
}
