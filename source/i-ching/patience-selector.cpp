#include "patience-selector.h"

namespace i_ching {

PatienceIChingSelector::PatienceIChingSelector(
    std::unique_ptr<medici::PatienceSelector>&& parent)
        : parent(std::move(parent)), checker(new BalanceChecker()) {}

PatienceIChingSelector::PatienceIChingSelector(
    std::uint_fast8_t suit, const Hexagram& targetHex,
    std::unique_ptr<medici::PatienceSelector>&& parent)
        : parent(std::move(parent)),
          checker(new BalanceAndSuitChecker(suit, targetHex)) {}

bool PatienceIChingSelector::Check(const std::vector<Card>& deck,
                                   const medici::PatienceInfo& info) {
    return parent->Check(deck, info) && checker->Check(info);
}
}
