#pragma once

#include "card.h"
#include "condition.h"

namespace cards {

class OneCardCondition : public Condition {
public:
    OneCardCondition(size_t position, const Card& card, bool inverse = false);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t position_;
    const Card card_;
    const bool inverse_;
};
}
