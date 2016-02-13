#include "move-condition.h"
#include <algorithm>

MoveCondition::MoveCondition(size_t position, const std::shared_ptr<Condition>& condition)
        : position_(position), condition_(condition) {}

std::vector<Sequence> MoveCondition::GetVariants(const Sequence& applied_sequence,
                                                 Storage storage) const {
    auto shifted_applied_sequence = applied_sequence.ShiftLeft(position_);
    auto variants = condition_->GetVariants(shifted_applied_sequence, storage);
    std::transform(
        variants.begin(), variants.end(), variants.begin(),
        [this](const Sequence& sequence) { return sequence.ShiftRight(position_); });
    return applied_sequence + variants;
}
