#ifndef MEDICI_PATIENCE_SELECTOR_H
#define MEDICI_PATIENCE_SELECTOR_H

#include <array>
#include <memory>
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
        PatienceTargetSelector(const Card& target, bool strictComparison);
        virtual bool Check(const Patience::PatienceInfo& info) override;
    protected:
        const Card target;
    private:
        bool strictComparison;
        std::size_t currentConvolutions = 0;
    };

    class PatienceIChingSelector : public PatienceSelector {
    public:
        PatienceIChingSelector(PPatienceSelector&& parent = PPatienceSelector());
        PatienceIChingSelector(const Card::Suit& suit, const i_ching::Hexagram& targetHex, PPatienceSelector&& parent = PPatienceSelector());
        virtual bool Check(const Patience::PatienceInfo&) override;
    private:
        PPatienceSelector parent;
        std::unique_ptr<i_ching::BalanceChecker> checker;
    };

}

#endif /* MEDICI_PATIENCE_SELECTOR_H */
