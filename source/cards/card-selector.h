#pragma once
#include "card.h"

class CardSelector {
public:
    bool Check(const Card&) const;

private:
    struct Config {
        std::uint_fast8_t suit;
        std::uint_fast8_t rank;
        bool suitSet;
        bool rankSet;
        bool straight;
    };
    const Config config_;
    CardSelector(Config&&);

    friend class CardSelectorConfigurator;
};

class CardSelectorConfigurator {
public:
    void SetSuit(std::uint_fast8_t);
    void SetRank(std::uint_fast8_t);
    void SetStraight(bool);
    void Reset();

    CardSelector GetSelector();

private:
    CardSelector::Config config_;
};
