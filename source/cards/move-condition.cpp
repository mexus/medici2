#include "move-condition.h"

#include <algorithm>

namespace cards {

MoveCondition::MoveCondition(size_t position, const std::shared_ptr<Condition>& condition)
        : position_(position), condition_(condition) {}

bool MoveCondition::CheckSequence(const std::vector<Card>& cards) const {
    if (position_ >= cards.size()) {
        return false;
    }
    std::vector<Card> shifted_deck(cards.begin() + position_, cards.end());
    return condition_->CheckSequence(shifted_deck);
}

std::vector<Sequence> MoveCondition::GetVariants(const Sequence& applied_sequence,
                                                 Storage storage) const {
    auto shifted_applied_sequence = applied_sequence.ShiftLeft(position_);
    auto variants = condition_->GetVariants(shifted_applied_sequence, storage);
    std::transform(
        variants.begin(), variants.end(), variants.begin(),
        [this](const Sequence& sequence) { return sequence.ShiftRight(position_); });
    return applied_sequence + variants;
}
}
