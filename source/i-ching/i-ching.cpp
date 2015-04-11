#include "i-ching.h"
#include <unordered_map>

namespace i_ching {

    SuitsHexagrams CalculateHexagrams(const medici::Patience::PatienceInfo& info)
    {
        using namespace standard_36_deck;
        static const Hexagram defaultHexagram{{Yang, Yang, Yang, Yang, Yang, Yang}};
        SuitsHexagrams hexagrams {{
            defaultHexagram,    // suit #0
            defaultHexagram,    // suit #1
            defaultHexagram,    // suit #2
            defaultHexagram     // suit #3
        }};
        static const std::unordered_map<std::uint_fast8_t, std::size_t> converter {
            {Ace,   6 - 1},
            {King,  5 - 1},
            {Queen, 4 - 1},// {Jack,  4 - 1},
            {Ten,   3 - 1},
            {Six,   2 - 1},// {Eight, 2 - 1},
            {Nine,  1 - 1},// {Seven, 1 - 1}
        };
        for (auto &card : info.stationars) {
            auto &suitHexagram = hexagrams[card.suit.value];
            auto it = converter.find(card.rank.value);
            if (it != converter.end()) {
                auto lineNumber = it->second;
                suitHexagram[lineNumber] = Yin;
            }
        }
        return hexagrams;
    }

    std::size_t IsYang(const Line& line)
    {
        return line == Yang ? 1 : 0;
    }

    bool CheckBalance(const Line& line1, const Line& line2, const Line& line3, const Line& line4)
    {
        std::size_t yangs = IsYang(line1) +  IsYang(line2) + IsYang(line3) + IsYang(line4);
        return yangs == 2;
    }

    bool CheckBalance(const SuitsHexagrams& hexagrams)
    {
        for (std::size_t line = 0; line != 6; ++line) {
            if (!CheckBalance(hexagrams[0][line], hexagrams[1][line], hexagrams[2][line], hexagrams[3][line]))
                return false;
        }
        return true;
    }

    bool BalanceChecker::Check(const medici::Patience::PatienceInfo& info) const
    {
        auto hexagrams = CalculateHexagrams(info);
        return CheckBalance(hexagrams);
    }

    BalanceAndSuitChecker::BalanceAndSuitChecker(const Card::Suit& suit, const Hexagram& hexagram) :
        suit(suit.value), hexagram(hexagram)
    {
    }

    bool BalanceAndSuitChecker::Check(const medici::Patience::PatienceInfo& info) const
    {
        auto hexagrams = CalculateHexagrams(info);
        return hexagrams[suit] == hexagram && CheckBalance(hexagrams);
    }

}

