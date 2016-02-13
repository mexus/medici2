#pragma once
#include <cards/card.h>

#include "condition.h"

class OneCardCondition : public Condition {
public:
    OneCardCondition(size_t position, const Card& card, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const Card card_;
    const bool inverse_;
};

class AnyRankCardCondition : public Condition {
public:
    AnyRankCardCondition(size_t position, uint_fast8_t suit, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const uint_fast8_t suit_;
    const bool inverse_;
};

class AnySuitCardCondition : public Condition {
public:
    AnySuitCardCondition(size_t position, uint_fast8_t rank, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const uint_fast8_t rank_;
    const bool inverse_;
};
