#pragma once

#include <array>

#include <cards/standard-36-deck.h>
#include <medici/patience.h>

namespace i_ching {

enum Line { Yang, Yin };
using Hexagram = std::array<Line, 6>;
using SuitsHexagrams = std::array<Hexagram, 4>;

SuitsHexagrams CalculateHexagrams(const medici::PatienceInfo& info);

class BalanceChecker {
public:
    virtual ~BalanceChecker() = default;

    virtual bool Check(const medici::PatienceInfo& info) const;
};

class BalanceAndSuitChecker : public BalanceChecker {
public:
    BalanceAndSuitChecker(std::uint_fast8_t suit, const Hexagram& hexagram);

    bool Check(const medici::PatienceInfo& info) const override;

private:
    const std::uint_fast8_t suit_;
    const Hexagram hexagram_;
};
}
