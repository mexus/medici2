#pragma once
#include "and-condition.h"
#include "move-condition.h"

namespace cards {

class ApplyToRangeCondition : public Condition {
public:
    ApplyToRangeCondition(size_t range_start, size_t range_end,
                          const std::shared_ptr<Condition>& condition);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    AndCondition condition_;
};
}
