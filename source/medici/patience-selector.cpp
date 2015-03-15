#include "patience-selector.h"

namespace medici {

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

