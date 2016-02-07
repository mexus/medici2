#include "patience-selector.h"

typedef std::lock_guard<std::mutex> Guard;
using namespace i_ching;

namespace medici {

PatienceTargetSelector::PatienceTargetSelector(const Card& target) : target(target) {
}

bool PatienceTargetSelector::Check(const std::vector<Card>& /*deck*/, const PatienceInfo& info) {
    auto it = info.convolutions.find(target);
    return it != info.convolutions.end() && it->second != 0;
}

PatienceMaxSelector::PatienceMaxSelector(const Card& target, bool strictComparison)
        : PatienceTargetSelector(target),
          strictComparison(strictComparison),
          maxConvolutions(0) {
}

bool PatienceMaxSelector::Check(const std::vector<Card>& /*deck*/, const PatienceInfo& info) {
    auto it = info.convolutions.find(target);
    if (it != info.convolutions.end() && it->second != 0) {
        std::size_t targetConvolutions = it->second;
        std::size_t max = maxConvolutions.load();
        if (targetConvolutions >= max) {
            Guard lock(accessConvolutions);
            max = maxConvolutions.load();
            if (targetConvolutions > max ||
                (!strictComparison && targetConvolutions == max)) {
                maxConvolutions.store(targetConvolutions);
                return true;
            }
        }
    }
    return false;
}

PatienceIChingSelector::PatienceIChingSelector(PPatienceSelector&& parent)
        : parent(std::move(parent)), checker(new BalanceChecker()) {
}

PatienceIChingSelector::PatienceIChingSelector(std::uint_fast8_t suit,
                                               const Hexagram& targetHex,
                                               PPatienceSelector&& parent)
        : parent(std::move(parent)), checker(new BalanceAndSuitChecker(suit, targetHex)) {
}

bool PatienceIChingSelector::Check(const std::vector<Card>& deck, const PatienceInfo& info) {
    return parent->Check(deck, info) && checker->Check(info);
}
}
