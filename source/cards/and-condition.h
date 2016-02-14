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
    /* Emperical formula for memory consumption:
     * Memory(Size) ~ -6.9541822e-13 * (Size)^2 + 1.8728709e-4 * Size + 5.556528
     * For example, for 1e7 (one billion) sequences an estimated memory consumption is
     * around 1.9 GB, and for one thousand sequences it is around 5.7 MB
     *
     * See "memory-consumption.md" for details
     */
    static const size_t kMaxSequences = 1e7;

    std::vector<std::shared_ptr<Condition>> child_conditions_;
};
}
