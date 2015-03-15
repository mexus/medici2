#include "patience-selector.h"

namespace medici {

    bool PatienceSelector::Check(const Patience::PatienceInfo&) {
        return true;
    }

    PatienceTargetSelector::PatienceTargetSelector(const Card& target, bool strictComparison) :
        target(target), strictComparison(strictComparison)
    {
    }

    bool PatienceTargetSelector::Check(const Patience::PatienceInfo& info) {
        auto it = info.convolutions.find(target);
        if (it != info.convolutions.end()) {
            std::size_t targetConvolutions = it->second;
            if ((strictComparison && targetConvolutions > currentConvolutions) ||
                (!strictComparison && targetConvolutions >= currentConvolutions))
            {
                currentConvolutions = targetConvolutions;
                return true;
            }
        }
        return false;
    }
}

