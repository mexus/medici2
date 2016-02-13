#include <i-ching/i-ching.h>

#include <unordered_map>

namespace {

inline size_t IsYang(const i_ching::Line& line) {
    return line == i_ching::Yang ? 1 : 0;
}

inline bool CheckBalance(const i_ching::Line& line1, const i_ching::Line& line2,
                         const i_ching::Line& line3, const i_ching::Line& line4) {
    std::size_t yangs = IsYang(line1) + IsYang(line2) + IsYang(line3) + IsYang(line4);
    return yangs == 2;
}

bool CheckBalance(const i_ching::SuitsHexagrams& hexagrams) {
    for (std::size_t line = 0; line != 6; ++line) {
        if (!CheckBalance(hexagrams[0][line], hexagrams[1][line], hexagrams[2][line],
                          hexagrams[3][line]))
            return false;
    }
    return true;
}
}

namespace i_ching {

SuitsHexagrams CalculateHexagrams(const medici::PatienceInfo& info) {
    using namespace standard_36_deck;
    static const Hexagram defaultHexagram{{Yang, Yang, Yang, Yang, Yang, Yang}};
    SuitsHexagrams hexagrams{{
        defaultHexagram,  // suit #0
        defaultHexagram,  // suit #1
        defaultHexagram,  // suit #2
        defaultHexagram   // suit #3
    }};
    static const std::unordered_map<std::uint_fast8_t, std::size_t> converter{
        {Ace, 6 - 1},  {King, 5 - 1}, {Queen, 4 - 1},  // {Jack,  4 - 1},
        {Ten, 3 - 1},  {Six, 2 - 1},                   // {Eight, 2 - 1},
        {Nine, 1 - 1},                                 // {Seven, 1 - 1}
    };
    for (auto& card : info.stationars) {
        auto& suitHexagram = hexagrams[card.suit];
        auto it = converter.find(card.rank);
        if (it != converter.end()) {
            auto lineNumber = it->second;
            suitHexagram[lineNumber] = Yin;
        }
    }
    return hexagrams;
}

bool BalanceChecker::Check(const medici::PatienceInfo& info) const {
    auto hexagrams = CalculateHexagrams(info);
    return Check(hexagrams);
}

bool BalanceChecker::Check(const SuitsHexagrams& hexagrams) const {
    return CheckBalance(hexagrams);
}

BalanceAndSuitChecker::BalanceAndSuitChecker(std::uint_fast8_t suit,
                                             const Hexagram& hexagram)
        : suit_(suit), hexagram_(hexagram) {}

bool BalanceAndSuitChecker::Check(const SuitsHexagrams& hexagrams) const {
    return hexagrams[suit_] == hexagram_ && CheckBalance(hexagrams);
}
}
