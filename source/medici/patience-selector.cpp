#include "patience-selector.h"

typedef std::lock_guard<std::mutex> Guard;

namespace medici {

    PatienceTargetSelector::PatienceTargetSelector(const Card& target) : target(target) {
    }

    bool PatienceTargetSelector::Check(const Patience::PatienceInfo& info) {
        auto it = info.convolutions.find(target);
        return it != info.convolutions.end() && it->second != 0;
    }

    PatienceMaxSelector::PatienceMaxSelector(const Card& target, bool strictComparison) :
        PatienceTargetSelector(target), strictComparison(strictComparison)
    {
    }

    bool PatienceMaxSelector::Check(const Patience::PatienceInfo& info) {
        auto it = info.convolutions.find(target);
        if (it != info.convolutions.end() && it->second != 0) {
            std::size_t targetConvolutions = it->second;
            std::size_t max = currentConvolutions.load();
            if (targetConvolutions >= max) {
                Guard lock(accessConvolutions);
                max = currentConvolutions.load();
                if (targetConvolutions > max || (!strictComparison && targetConvolutions == max)) {
                    currentConvolutions.store(targetConvolutions);
                    return true;
                }
            }
        }
        return false;
    }

    PatienceIChingSelector::PatienceIChingSelector(PPatienceSelector&& parent) :
        parent(std::move(parent)), checker(new i_ching::BalanceChecker())
    {
    }

    PatienceIChingSelector::PatienceIChingSelector(const Card::Suit& suit, const i_ching::Hexagram& targetHex, PPatienceSelector&& parent) : 
        parent(std::move(parent)), checker(new i_ching::BalanceAndSuitChecker(suit, targetHex))
    {
    }

    bool PatienceIChingSelector::Check(const Patience::PatienceInfo& info) {
            return parent->Check(info) && checker->Check(info);
    }

}

