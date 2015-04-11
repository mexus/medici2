#ifndef I_CHING_I_CHING_H
#define I_CHING_I_CHING_H

#include <cards/standard-36-deck.h>
#include <medici/patience.h>
#include <array>

namespace i_ching {

    enum Line {
        Yang,
        Yin
    };
    typedef std::array<Line, 6> Hexagram;
    typedef std::array<Hexagram, 4> SuitsHexagrams;

    struct BalanceChecker{
        virtual bool Check(const medici::Patience::PatienceInfo&) const;
        virtual ~BalanceChecker() = default;
    };

    class BalanceAndSuitChecker : public BalanceChecker{
    public:
        BalanceAndSuitChecker(const Card::Suit& suit, const Hexagram&);
        bool Check(const medici::Patience::PatienceInfo&) const override;
    private:
        std::uint_fast8_t suit;
        Hexagram hexagram;
    };

    SuitsHexagrams CalculateHexagrams(const medici::Patience::PatienceInfo& info);

}

#endif /* I_CHING_I_CHING_H */
