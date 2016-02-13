#pragma once

#include "condition.h"

class MoveCondition : public Condition {
public:
    MoveCondition(size_t position, const std::shared_ptr<Condition>& condition);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    std::shared_ptr<Condition> condition_;
};
