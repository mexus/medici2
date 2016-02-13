#pragma once
#include <cstdlib>
#include <memory>
#include <vector>

#include "card.h"
#include "sequence.h"
#include "storage.h"

namespace cards {

class Condition {
public:
    virtual std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                              Storage storage) const = 0;
    virtual bool CheckSequence(const std::vector<Card>& cards) const = 0;
};
}
