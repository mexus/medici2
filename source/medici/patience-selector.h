#pragma once

#include <cards/card.h>
#include "patience.h"

namespace medici {

class PatienceSelector {
public:
    virtual ~PatienceSelector() = default;
    virtual bool Check(const std::vector<Card>& deck, const PatienceInfo& info);
};

// class PatienceIChingSelector : public PatienceSelector {
// public:
//     PatienceIChingSelector(PPatienceSelector&& parent);
//     PatienceIChingSelector(std::uint_fast8_t suit, const i_ching::Hexagram& targetHex,
//                            PPatienceSelector&& parent);
//     virtual bool Check(const std::vector<Card>& /*deck*/, const PatienceInfo&)
//     override;
//
// private:
//     const PPatienceSelector parent;
//     const std::unique_ptr<const i_ching::BalanceChecker> checker;
// };
}
