#pragma once

#include "card.h"
#include "condition.h"

namespace cards {

class AnySuitCondition : public Condition {
public:
    AnySuitCondition(size_t position, uint_fast8_t rank, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const uint_fast8_t rank_;
    const bool inverse_;
};
}
