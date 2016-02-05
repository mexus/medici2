#include "ace-selector.h"
#include <cards/standard-36-deck.h>

AceSelector::AceSelector() {}

bool AceSelector::Check(const std::vector<Card> &deck) const {
    static const standard_36_deck::Ranks ace = standard_36_deck::Ace;
    for (size_t i = 0; i < deck.size() - 3; ++i) {
        if (deck[i].rank != ace) {
            continue;
        }
        auto &second = deck[i + 1];
        auto &third = deck[i + 2];
        auto &forth = deck[i + 3];
        if (second.rank != ace || third.rank != ace || forth.rank != ace) {
            return false;
        }
        return forth.suit == standard_36_deck::Spades ||
               forth.suit == standard_36_deck::Hearts;
    }
    return false;
}
