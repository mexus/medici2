#include "card-selector.h"
#include <utility>

CardSelector::CardSelector(Config&& config) : config_(config) {
}

void CardSelectorConfigurator::SetSuit(std::uint_fast8_t suit) {
    config_.suitSet = true;
    config_.suit = suit;
}

void CardSelectorConfigurator::SetRank(std::uint_fast8_t rank) {
    config_.rankSet = true;
    config_.rank = rank;
}

void CardSelectorConfigurator::SetStraight(bool is_straight) {
    config_.straight = is_straight;
}

bool CardSelector::Check(const Card& card) const {
    bool match = (!config_.rankSet || config_.rank == card.rank) &&
                 (!config_.suitSet || config_.suit == card.suit);
    return config_.straight == match;
}

CardSelector CardSelectorConfigurator::GetSelector() {
    return CardSelector(std::move(config_));
}

void CardSelectorConfigurator::Reset() {
    config_.rankSet = false;
    config_.suitSet = false;
    config_.straight = true;
}
