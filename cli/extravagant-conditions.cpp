#include "extravagant-conditions.h"
#include <algorithm>

#include <cards/standard-36-deck.h>

size_t ExtravagantPatience::GetConvolutions(const std::map<Card, size_t>& convolutions,
                                            const Card& card) {
    auto it = convolutions.find(card);
    if (it == convolutions.end()) {
        return 0;
    }
    return it->second;
}

bool ExtravagantPatience::IsAce(const Card& card) {
    return card.rank == standard_36_deck::Ace;
}

bool ExtravagantPatience::Check(const std::vector<Card>& deck,
                                const medici::PatienceInfo& info) {
    size_t convolutions_chains = 0;
    for (auto& pair : info.convolutions) {
        size_t convolutions = pair.second;
        if (convolutions != 0) {
            convolutions_chains++;
        }
    }
    if (convolutions_chains > 8) {
        return false;
    }
    auto forth_ace_it = std::find_if(deck.rbegin(), deck.rend(), IsAce);
    if (forth_ace_it == deck.rend()) {
        throw std::logic_error("No ace found!");
    }
    ssize_t distance_from_the_beginning = std::distance(forth_ace_it, deck.rend());
    if (distance_from_the_beginning < 4) {
        throw std::logic_error("The 4th ace is too close to the begining of the deck");
    }
    if (GetConvolutions(info.convolutions, *forth_ace_it) < 7) {
        return false;
    }
    auto third_ace_it = forth_ace_it + 1;
    if (third_ace_it == deck.rend() || !IsAce(*third_ace_it)) {
        throw std::logic_error("Third ace not found...");
    }
    size_t third_ace_convolutions = GetConvolutions(info.convolutions, *third_ace_it);
    if (third_ace_convolutions != 3 && third_ace_convolutions != 4) {
        return false;
    }
    return true;
}
