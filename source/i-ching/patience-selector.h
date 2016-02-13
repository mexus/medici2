#pragma once
#include <memory>

#include <medici/patience-selector.h>

#include "i-ching.h"

namespace i_ching {

class PatienceIChingSelector : public medici::PatienceSelector {
public:
    PatienceIChingSelector(std::unique_ptr<medici::PatienceSelector>&& parent);
    PatienceIChingSelector(std::uint_fast8_t suit, const i_ching::Hexagram& targetHex,
                           std::unique_ptr<PatienceSelector>&& parent);
    virtual bool Check(const std::vector<Card>& deck,
                       const medici::PatienceInfo& info) override;

private:
    const std::unique_ptr<PatienceSelector> parent;
    const std::unique_ptr<const i_ching::BalanceChecker> checker;
};
}
