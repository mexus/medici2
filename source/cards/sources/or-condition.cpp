#include <cards/or-condition.h>

namespace cards {

bool OrCondition::CheckSequence(const std::vector<Card>& cards) const {
    for (auto& condition : child_conditions_) {
        if (condition->CheckSequence(cards)) {
            return true;
        }
    }
    return false;
}

std::vector<Sequence> OrCondition::GetVariants(const Sequence& applied_sequence,
                                               Storage storage) const {
    std::vector<Sequence> result;
    for (auto& condition : child_conditions_) {
        auto variants = condition->GetVariants(applied_sequence, storage);
        result.insert(result.end(), std::make_move_iterator(variants.begin()),
                      std::make_move_iterator(variants.end()));
    }
    return result;
}

void OrCondition::AddChild(const std::shared_ptr<Condition>& condition) {
    child_conditions_.push_back(condition);
}
}
