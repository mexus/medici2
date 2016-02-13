#pragma once

#include <cards/card.h>
#include "patience.h"

namespace medici {

class PatienceSelector {
public:
    virtual ~PatienceSelector() = default;
    virtual bool Check(const std::vector<cards::Card>& deck, const PatienceInfo& info);
};
}
