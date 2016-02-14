#pragma once

#include "condition.h"

namespace cards {

class AndCondition : public Condition {
public:
    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

    void AddChild(const std::shared_ptr<Condition>& condition);

    template <class T, class... Args>
    void AddChild(Args&&... args) {
        AddChild(std::make_shared<T>(std::forward<Args>(args)...));
    }

private:
    static const size_t kMaxSequences = 100000;

    std::vector<std::shared_ptr<Condition>> child_conditions_;
};
}
