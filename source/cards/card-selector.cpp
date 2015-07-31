#include "card-selector.h"
#include <utility>

CardSelector::CardSelector(Config&& config) : config(config) {
}

void CardSelectorConfigurator::SetSuit(std::uint_fast8_t suit) {
    config.suitSet = true;
    config.suit = suit;
}

void CardSelectorConfigurator::SetRank(std::uint_fast8_t rank) {
    config.rankSet = true;
    config.rank = rank;
}

void CardSelectorConfigurator::SetStraight(bool is_straight) {
    config.straight = is_straight;
}

bool CardSelector::Check(const Card& card) const {
    bool match = (!config.rankSet || config.rank == card.rank) &&
                 (!config.suitSet || config.suit == card.suit);
    return config.straight == match;
}

CardSelector CardSelectorConfigurator::GetSelector() {
    return CardSelector(std::move(config));
}

void CardSelectorConfigurator::Reset() {
    config.rankSet = false;
    config.suitSet = false;
    config.straight = true;
}
