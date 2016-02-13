#include "and-condition.h"

void AndCondition::AddChild(const std::shared_ptr<Condition>& condition) {
    child_conditions_.push_back(condition);
}

std::vector<Sequence> AndCondition::GetVariants(const Sequence& applied_sequence,
                                                Storage storage) const {
    if (child_conditions_.empty()) {
        return {};
    }

    std::vector<Sequence> result{applied_sequence};
    for (auto& condition : child_conditions_) {
        std::vector<Sequence> intermediate_sequences;
        for (auto& original_sequence : result) {
            Storage storage_copy(storage);
            original_sequence.TakeCardsFromStorage(&storage_copy);
            auto variants = condition->GetVariants(original_sequence, storage_copy);
            auto tmp = original_sequence + variants;
            intermediate_sequences.insert(intermediate_sequences.end(),
                                          std::make_move_iterator(tmp.begin()),
                                          std::make_move_iterator(tmp.end()));
        }
        result = std::move(intermediate_sequences);
    }

    return result;
}
