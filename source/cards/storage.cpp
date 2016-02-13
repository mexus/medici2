#include "storage.h"

namespace cards {

Storage::Storage(const std::vector<Card>& cards) {
    for (auto& card : cards) {
        cards_by_rank_[card.rank].insert(card.suit);
        cards_by_suit_[card.suit].insert(card.rank);
    }
}

Storage::Storage(const Storage& other)
        : cards_by_rank_(other.cards_by_rank_), cards_by_suit_(other.cards_by_suit_) {}

std::set<uint_fast8_t> Storage::GetRanksForSuit(uint_fast8_t suit) const {
    auto it = cards_by_suit_.find(suit);
    if (it == cards_by_suit_.end()) {
        return {};
    }
    return it->second;
}

std::set<uint_fast8_t> Storage::GetSuitsForRank(uint_fast8_t rank) const {
    auto it = cards_by_rank_.find(rank);
    if (it == cards_by_rank_.end()) {
        return {};
    }
    return it->second;
}

bool Storage::HasCard(const Card& card) const {
    auto rank_it = cards_by_rank_.find(card.rank);
    if (rank_it == cards_by_rank_.end()) {
        return false;
    }
    auto& suits = rank_it->second;
    return suits.count(card.suit);
}

void Storage::TakeCard(const Card& card) {
    cards_by_rank_[card.rank].erase(card.suit);
    cards_by_suit_[card.suit].erase(card.rank);
}

std::vector<Card> Storage::GetAllAvailableCards() const {
    std::vector<Card> result;
    for (auto& pair : cards_by_rank_) {
        uint_fast8_t rank = pair.first;
        auto& suits = pair.second;
        for (auto& suit : suits) {
            result.push_back({suit, rank});
        }
    }
    return result;
}
}
