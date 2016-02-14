#include <gtest/gtest.h>

#include <cards/and-condition.h>

using namespace cards;

namespace {
class FakeCheckCondition : public Condition {
public:
    FakeCheckCondition(std::vector<Card> cards, bool check)
            : cards_(cards), check_(check) {}

    std::vector<Sequence> GetVariants(const Sequence& /*applied_sequence*/,
                                      Storage /*storage*/) const override {
        return {};
    }

    bool CheckSequence(const std::vector<Card>& cards) const override {
        if (cards != cards_) {
            throw std::logic_error("Pass wrong cards: " +
                                   ::testing::PrintToString(cards));
        }
        return check_;
    }

private:
    std::vector<Card> cards_;
    bool check_;
};
}

TEST(Cards_AndCondition, CheckSequenceAllTrue) {
    AndCondition and_condition;
    const std::vector<Card> cards{{0, 1}, {12, 91}, {46, 92}};
    and_condition.AddChild<FakeCheckCondition>(cards, true);
    and_condition.AddChild<FakeCheckCondition>(cards, true);
    ASSERT_TRUE(and_condition.CheckSequence(cards));
}

TEST(Cards_AndCondition, CheckSequenceAllFalse) {
    AndCondition and_condition;
    const std::vector<Card> cards{{0, 1}, {12, 91}, {46, 92}};
    and_condition.AddChild<FakeCheckCondition>(cards, false);
    and_condition.AddChild<FakeCheckCondition>(cards, false);
    ASSERT_FALSE(and_condition.CheckSequence(cards));
}

TEST(Cards_AndCondition, CheckSequenceFirstFalse) {
    AndCondition and_condition;
    const std::vector<Card> cards{{0, 1}, {12, 91}, {46, 92}};
    and_condition.AddChild<FakeCheckCondition>(cards, false);
    and_condition.AddChild<FakeCheckCondition>(cards, true);
    ASSERT_FALSE(and_condition.CheckSequence(cards));
}

TEST(Cards_AndCondition, CheckSequenceSecondFalse) {
    AndCondition and_condition;
    const std::vector<Card> cards{{0, 1}, {12, 91}, {46, 92}};
    and_condition.AddChild<FakeCheckCondition>(cards, true);
    and_condition.AddChild<FakeCheckCondition>(cards, false);
    ASSERT_FALSE(and_condition.CheckSequence(cards));
}

namespace {

using Deck = std::vector<Card>;
using Decks = std::vector<Deck>;

class FakeVariantsCondition : public Condition {
public:
    FakeVariantsCondition(const Decks& variants)
            : variants_(variants) {}

    std::vector<Sequence> GetVariants(const Sequence& /*applied_sequence*/,
                                      Storage /*storage*/) const override {
        std::vector<Sequence> result;
        for (auto& var : variants_) {
            result.emplace_back(SimpleSequence{{var}, 0});
        }
        return result;
    }

    bool CheckSequence(const std::vector<Card>& /*cards*/) const override {
        return false;
    }

private:
    Decks variants_;
};
}

TEST(Cards_AndCondition, GetVariantsSame) {
    AndCondition and_condition;
    FakeVariantsCondition v1({{{0, 0}, {0, 1}}, {{0, 0}, {0, 2}}});
    and_condition.AddChild<FakeVariantsCondition>(v1);
    and_condition.AddChild<FakeVariantsCondition>(v1);
    auto variants = and_condition.GetVariants({}, Storage({}));

    ASSERT_EQ(2, variants.size());
    ASSERT_EQ(1, variants[0].GetParts().size());
    ASSERT_EQ(0, variants[0].GetParts()[0].position);
    ASSERT_EQ(Deck({{0, 0}, {0, 1}}), variants[0].GetParts()[0].cards);
    ASSERT_EQ(1, variants[1].GetParts().size());
    ASSERT_EQ(0, variants[1].GetParts()[0].position);
    ASSERT_EQ(Deck({{0, 0}, {0, 2}}), variants[1].GetParts()[0].cards);
}

TEST(Cards_AndCondition, GetVariantsContradictoryApplied) {
    AndCondition and_condition;
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{0, 0}, {0, 1}}, {{0, 0}, {0, 2}}});
    auto variants = and_condition.GetVariants(SimpleSequence{{{0, 1}}, 0}, Storage({}));

    ASSERT_TRUE(variants.empty());
}

TEST(Cards_AndCondition, GetVariantsContradictory) {
    AndCondition and_condition;
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{0, 1}, {0, 2}}});
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{1, 1}}});
    auto variants = and_condition.GetVariants({}, Storage({}));

    ASSERT_TRUE(variants.empty());
}

TEST(Cards_AndCondition, GetVariants) {
    AndCondition and_condition;
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{0, 1}, {0, 2}}});
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{1, 1}}, {{0, 1}}});
    and_condition.AddChild<FakeVariantsCondition>(Decks{{{0, 1}, {0, 2}, {4, 5}}, {{0, 1}, {0, 2}, {5, 6}}});
    auto variants = and_condition.GetVariants({}, Storage({}));

    ASSERT_EQ(2, variants.size());
    ASSERT_EQ(1, variants[0].GetParts().size());
    ASSERT_EQ(0, variants[0].GetParts()[0].position);
    ASSERT_EQ(Deck({{0, 1}, {0, 2}, {4, 5}}), variants[0].GetParts()[0].cards);
    ASSERT_EQ(1, variants[1].GetParts().size());
    ASSERT_EQ(0, variants[1].GetParts()[0].position);
    ASSERT_EQ(Deck({{0, 1}, {0, 2}, {5, 6}}), variants[1].GetParts()[0].cards);
}
