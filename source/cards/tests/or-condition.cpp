#include <gtest/gtest.h>

#include <cards/or-condition.h>

#include "print.h"

using namespace cards;

struct FakeCondition : public Condition {
public:
    FakeCondition(Sequence result) : result_(result) {}

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override {
        (void)applied_sequence;
        (void)storage;
        return {result_};
    }

    bool CheckSequence(const std::vector<Card>& cards) const override {
        return !result_.Contradicts(SimpleSequence{cards, 0});
    }

private:
    const Sequence result_;
};

TEST(Cards_OrCondition, Check) {
    OrCondition or_condition;
    or_condition.AddChild<FakeCondition>(SimpleSequence{{{0, 1}}, 0});
    or_condition.AddChild<FakeCondition>(SimpleSequence{{{2, 0}}, 0});

    ASSERT_TRUE(or_condition.CheckSequence({{0, 1}, {0, 2}}));
    ASSERT_FALSE(or_condition.CheckSequence({{0, 0}, {0, 1}, {0, 2}}));
    ASSERT_TRUE(or_condition.CheckSequence({{2, 0}, {0, 2}}));
    ASSERT_FALSE(or_condition.CheckSequence({{0, 0}, {2, 0}, {0, 2}}));
}

TEST(Cards_OrCondition, GetVariants) {
    OrCondition or_condition;
    or_condition.AddChild<FakeCondition>(SimpleSequence{{{0, 1}}, 0});
    or_condition.AddChild<FakeCondition>(SimpleSequence{{{2, 0}}, 1});

    Storage empty_storage({});

    std::vector<Sequence> etalon{SimpleSequence{{{0, 1}}, 0},
                                 SimpleSequence{{{2, 0}}, 1}};
    ASSERT_EQ(etalon, or_condition.GetVariants(SimpleSequence{{{}}, 0}, empty_storage));
}
