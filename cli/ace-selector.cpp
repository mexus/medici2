#include "ace-selector.h"
#include <algorithm>
#include <cards/standard-36-deck.h>

bool AceSelector::IsAce(const Card &card) {
    return card.rank == standard_36_deck::Ace;
}

AceSelector::AceSelector() {}

bool AceSelector::Check(const std::vector<Card> &deck) const {
    auto it = std::find_if(deck.begin(), deck.end(), IsAce);
    if (it == deck.end()) {
        throw std::logic_error("No ace on the deck");
    }
    ssize_t first_ace_distance = std::distance(it, deck.end());
    // Positions are counted from 0: in "Yy Zz", "Zz" is the 1st card.
    // The third ACE should converge at least 3 cards, so it should be at least 4th in the
    // deck. The forth ACE should converge at least 7 cards, so it should be at least 8th
    // in the deck. But the 3rd ACE will "eat" at least 3 cards, so the 4th ACE should be
    // at least (8+3)th in the deck. So the first ACE should be at least ((8+3)-3)th.
    //
    // For example, if the 4th ACE position is 7, then the 3rd ACE's is 6, 2nd is 5 and
    // 1st is 4:
    // Yy Yy Yy Yy Тп Тк Тб Тч ...

    ssize_t minimal_distance = (8 + 3) - 3;
    if (first_ace_distance < minimal_distance) {
        return false;
    }
    auto &first_card = *it;
    auto &second_card = *(++it);
    auto &third_card = *(++it);
    auto &forth_card = *(++it);

    if (!IsAce(second_card) || !IsAce(third_card) || !IsAce(forth_card)) {
        return false;
    }

    // We will try to find only one combination: Diamonds, Clubs, Hearts, Spades.
    // All other aces combinations can be derived by changing of suits names.
    return first_card.suit == standard_36_deck::Diamonds &&
           second_card.suit == standard_36_deck::Clubs &&
           third_card.suit == standard_36_deck::Hearts &&
           forth_card.suit == standard_36_deck::Spades;
}
