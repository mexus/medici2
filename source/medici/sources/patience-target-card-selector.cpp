#include <medici/patience-target-card-selector.h>

using cards::Card;

namespace medici {

PatienceTargetSelector::PatienceTargetSelector(const Card& target) : target_(target) {}

bool PatienceTargetSelector::Check(const std::vector<Card>& /*deck*/,
                                   const PatienceInfo& info) {
    auto it = info.convolutions.find(target_);
    return it != info.convolutions.end() && it->second != 0;
}

PatienceMaxSelector::PatienceMaxSelector(const Card& target, bool strict_comparison)
        : target_(target), strict_comparison_(strict_comparison), max_convolutions_(0) {}

bool PatienceMaxSelector::Check(const std::vector<Card>& /*deck*/,
                                const PatienceInfo& info) {
    auto it = info.convolutions.find(target_);
    if (it == info.convolutions.end() || it->second == 0) {
        return false;
    }
    std::size_t target_convolutions = it->second;
    size_t max = max_convolutions_.load();
    if (target_convolutions < max) {
        return false;
    }
    std::lock_guard<std::mutex> lg(access_convolutions_);
    /* While the mutex wasn't obtained, the "max convolutions" might have been changed */
    max = max_convolutions_.load();
    if (target_convolutions < max) {
        return false;
    }
    if (strict_comparison_ && target_convolutions == max) {
        return false;
    }
    max_convolutions_.store(target_convolutions);
    return true;
}
}
