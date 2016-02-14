#include <gtest/gtest.h>

#include <cards/move-condition.h>

using namespace cards;

TEST(Cards_MoveCondition, CheckOutOfRange) {
    MoveCondition condition(2, {});
    std::vector<Card> deck{{0, 0}, {0, 1}};
    ASSERT_FALSE(condition.CheckSequence(deck));
}

namespace {

struct FakeCheck : Condition {
    std::vector<Sequence> GetVariants(const Sequence& /*applied_sequence*/,
                                      Storage /*storage*/) const override {
        return {};
    }

    bool CheckSequence(const std::vector<Card>& cards) const override {
        checked_deck = cards;
        return true;
    }

    mutable std::vector<Card> checked_deck;
};
}

TEST(Cards_MoveCondition, Check) {
    std::vector<Card> deck{{0, 0}, {0, 1}, {0, 2}, {0, 3}};

    auto checker = std::make_shared<FakeCheck>();
    MoveCondition condition(2, checker);

    condition.CheckSequence(deck);
    ASSERT_EQ(std::vector<Card>({{0, 2}, {0, 3}}), checker->checked_deck);
}
