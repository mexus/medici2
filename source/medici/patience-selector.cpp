#include "patience-selector.h"

namespace medici {

bool PatienceSelector::Check(const std::vector<Card>& /*deck*/,
                             const PatienceInfo& /*info*/) {
    return true;
}

// PatienceIChingSelector::PatienceIChingSelector(PPatienceSelector&& parent)
//         : parent(std::move(parent)), checker(new BalanceChecker()) {}
//
// PatienceIChingSelector::PatienceIChingSelector(std::uint_fast8_t suit,
//                                                const Hexagram& targetHex,
//                                                PPatienceSelector&& parent)
//         : parent(std::move(parent)),
//           checker(new BalanceAndSuitChecker(suit, targetHex)) {}
//
// bool PatienceIChingSelector::Check(const std::vector<Card>& deck,
//                                    const PatienceInfo& info) {
//     return parent->Check(deck, info) && checker->Check(info);
// }
}
