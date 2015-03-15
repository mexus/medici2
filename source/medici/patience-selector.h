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
        virtual bool Check(const Patience::PatienceInfo&);
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
        virtual bool Check(const Patience::PatienceInfo& info) override {
            return checker.Check(info);
        }
    private:
        i_ching::BalanceChecker checker;
    };

    class PatienceIChingSuitSelector : public PatienceSelector {
    public:
        PatienceIChingSuitSelector(const Card::Suit& suit, const i_ching::Hexagram& targetHex) : checker(suit, targetHex) {}
        virtual bool Check(const Patience::PatienceInfo& info) override {
            return checker.Check(info);
        }
    private:
        i_ching::BalanceAndSuitChecker checker;
    };

}

#endif /* MEDICI_PATIENCE_SELECTOR_H */
