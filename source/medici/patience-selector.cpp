#include "patience-selector.h"

namespace medici {

bool PatienceSelector::Check(const std::vector<cards::Card>& /*deck*/,
                             const PatienceInfo& /*info*/) {
    return true;
}
}
