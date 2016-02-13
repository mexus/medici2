#pragma once
#include <cstdlib>
#include <memory>
#include <vector>
#include <cards/card.h>
#include "storage.h"
#include "sequence.h"

class Condition {
public:
    virtual std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                              Storage storage) const = 0;
};
