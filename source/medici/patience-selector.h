#ifndef MEDICI_PATIENCE_SELECTOR_H
#define MEDICI_PATIENCE_SELECTOR_H

#include <array>
#include <memory>
#include <atomic>
#include <mutex>
#include <cards/deck.h>
#include <i-ching/i-ching.h>
#include "patience.h"

namespace medici {

    class PatienceSelector {
    public:
        virtual ~PatienceSelector() = default;
        virtual bool Check(const Patience::PatienceInfo&) { return true; }
    };
    typedef std::unique_ptr<PatienceSelector> PPatienceSelector;

    class PatienceTargetSelector : public PatienceSelector {
    public:
        PatienceTargetSelector(const Card& target);
        virtual ~PatienceTargetSelector() = default;
        virtual bool Check(const Patience::PatienceInfo&);
    protected:
        const Card target;
    };

    class PatienceMaxSelector : public PatienceTargetSelector {
    public:
        PatienceMaxSelector(const Card& target, bool strictComparison);
        virtual bool Check(const Patience::PatienceInfo& info) override;
    private:
        const bool strictComparison;
        std::mutex accessConvolutions;
        std::atomic_size_t currentConvolutions;
    };

    class PatienceIChingSelector : public PatienceSelector {
    public:
        PatienceIChingSelector(PPatienceSelector&& parent = PPatienceSelector());
        PatienceIChingSelector(const Card::Suit& suit, const i_ching::Hexagram& targetHex, PPatienceSelector&& parent = PPatienceSelector());
        virtual bool Check(const Patience::PatienceInfo&) override;
    private:
        const PPatienceSelector parent;
        const std::unique_ptr<const i_ching::BalanceChecker> checker;
    };

}

#endif /* MEDICI_PATIENCE_SELECTOR_H */
