#pragma once
#include <map>
#include <set>
#include <vector>
#include <cards/card.h>

class Storage {
public:
    Storage(const std::vector<Card>& cards);
    Storage(const Storage& other);

    std::set<uint_fast8_t> GetRanksForSuit(uint_fast8_t suit) const;
    std::set<uint_fast8_t> GetSuitsForRank(uint_fast8_t rank) const;
    bool HasCard(const Card& card) const;
    void TakeCard(const Card& card);
    std::vector<Card> GetAllAvailableCards() const;

private:
    std::map<uint_fast8_t, std::set<uint_fast8_t>> cards_by_rank_, cards_by_suit_;
};
