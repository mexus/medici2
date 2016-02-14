#pragma once

#include "card.h"
#include "condition.h"

namespace cards {

class AnyRankCondition : public Condition {
public:
    AnyRankCondition(size_t position, uint_fast8_t suit, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const uint_fast8_t suit_;
    const bool inverse_;
};
}
