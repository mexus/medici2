#ifndef MEDICI_PATIENCE_SELECTOR_H
#define MEDICI_PATIENCE_SELECTOR_H

#include <array>
#include <memory>
#include <atomic>
#include <mutex>
#include <cards/card.h>
#include <i-ching/i-ching.h>
#include "patience.h"

namespace medici {

class PatienceSelector {
public:
    virtual ~PatienceSelector() = default;
    virtual bool Check(const std::vector<Card>& /*deck*/, const PatienceInfo& /*info*/) {
        return true;
    }
};
typedef std::unique_ptr<PatienceSelector> PPatienceSelector;

class PatienceTargetSelector : public PatienceSelector {
public:
    PatienceTargetSelector(const Card& target);
    virtual ~PatienceTargetSelector() = default;
    virtual bool Check(const std::vector<Card>& /*deck*/, const PatienceInfo&);

protected:
    const Card target;
};

class PatienceMaxSelector : public PatienceTargetSelector {
public:
    PatienceMaxSelector(const Card& target, bool strictComparison);
    virtual bool Check(const std::vector<Card>& /*deck*/, const PatienceInfo& info) override;

private:
    const bool strictComparison;
    std::mutex accessConvolutions;
    std::atomic_size_t maxConvolutions;
};

class PatienceIChingSelector : public PatienceSelector {
public:
    PatienceIChingSelector(PPatienceSelector&& parent);
    PatienceIChingSelector(std::uint_fast8_t suit, const i_ching::Hexagram& targetHex,
                           PPatienceSelector&& parent);
    virtual bool Check(const std::vector<Card>& /*deck*/, const PatienceInfo&) override;

private:
    const PPatienceSelector parent;
    const std::unique_ptr<const i_ching::BalanceChecker> checker;
};
}

#endif /* MEDICI_PATIENCE_SELECTOR_H */
