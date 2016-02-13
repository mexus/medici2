#include "apply-to-range-condition.h"
#include <cassert>
#include <easylogging++.h>

ApplyToRangeCondition::ApplyToRangeCondition(
    size_t range_start, size_t range_end, const std::shared_ptr<Condition>& condition) {
    assert(range_end >= range_start &&
           "`range_end` should not be less than `range_start`");
    for (size_t position = range_start; position <= range_end; ++position) {
        condition_.AddChild<MoveCondition>(position, condition);
    }
}

bool ApplyToRangeCondition::CheckSequence(const std::vector<Card>& cards) const {
    return condition_.CheckSequence(cards);
}

std::vector<Sequence> ApplyToRangeCondition::GetVariants(const Sequence& applied_sequence,
                                                         Storage storage) const {
    return condition_.GetVariants(applied_sequence, storage);
}
