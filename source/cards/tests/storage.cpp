#include <algorithm>

#include <gtest/gtest.h>

#include <cards/storage.h>

#include "print.h"

using namespace cards;

namespace {
std::vector<Card> Sort(const std::vector<Card>& cards) {
    std::vector<Card> result(cards);
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<Card> Unique(const std::vector<Card>& cards) {
    std::vector<Card> result(cards);
    std::sort(result.begin(), result.end());
    auto last = std::unique(result.begin(), result.end());
    result.erase(last, result.end());
    return result;
}

template <class Suit, class Rank>
std::vector<Card> SuitMapToVector(const std::map<Suit, std::set<Rank>>& map) {
    std::vector<Card> cards;
    for (auto& pair : map) {
        Suit suit = pair.first;
        for (auto& rank : pair.second) {
            cards.push_back(Card{suit, rank});
        }
    }
    return cards;
}

template <class Rank, class Suit>
std::vector<Card> RankMapToVector(const std::map<Rank, std::set<Suit>>& map) {
    std::vector<Card> cards;
    for (auto& pair : map) {
        Rank rank = pair.first;
        for (auto& suit : pair.second) {
            cards.push_back(Card{suit, rank});
        }
    }
    return cards;
}

template <class T>
std::set<uint_fast8_t> Convert(const std::set<T>& set) {
    return std::set<uint_fast8_t>(set.begin(), set.end());
}
}

TEST(Cards_Storage, Available) {
    static const std::vector<Card> cards1{{0, 0}, {3, 5}, {10, 11}};
    const Storage storage1(cards1);

    static const std::vector<Card> cards2{{4, 18}, {11, 123}, {0, 1}, {99, 99}, {1, 1}};
    const Storage storage2(cards2);

    static const std::vector<Card> non_unique_cards{{4, 18},  {11, 123}, {0, 1},
                                                    {99, 99}, {1, 1},    {11, 123}};
    const Storage storage3(non_unique_cards);

    ASSERT_EQ(Sort(cards1), Sort(storage1.GetAllAvailableCards()));
    ASSERT_EQ(Sort(cards2), Sort(storage2.GetAllAvailableCards()));
    ASSERT_NE(Sort(non_unique_cards), Sort(storage3.GetAllAvailableCards()));
    ASSERT_EQ(Unique(non_unique_cards), Sort(storage3.GetAllAvailableCards()));
}

TEST(Cards_Storage, Copy) {
    static const std::vector<Card> cards{{0, 0}, {3, 5}, {10, 11}};
    const Storage storage(cards);
    const Storage copy(storage);
    ASSERT_EQ(cards, copy.GetAllAvailableCards());
}

TEST(Cards_Storage, GetRanksForSuit) {
    enum Rank : uint_fast8_t { RANK1, RANK2, RANK3 };
    enum Suit : uint_fast8_t { SUIT1, SUIT2, SUIT3 };
    std::map<Suit, std::set<Rank>> cards_map{
        {SUIT1, {RANK1, RANK2, RANK3}}, {SUIT2, {RANK1, RANK3}}, {SUIT3, {}}};
    auto cards = SuitMapToVector(cards_map);
    const Storage storage(cards);
    ASSERT_EQ(Convert(cards_map[SUIT1]), storage.GetRanksForSuit(SUIT1));
    ASSERT_EQ(Convert(cards_map[SUIT2]), storage.GetRanksForSuit(SUIT2));
    ASSERT_TRUE(storage.GetRanksForSuit(SUIT3).empty());
}

TEST(Cards_Storage, GetSuitsForRank) {
    enum Rank : uint_fast8_t { RANK1, RANK2, RANK3 };
    enum Suit : uint_fast8_t { SUIT1, SUIT2, SUIT3 };
    std::map<Rank, std::set<Suit>> cards_map{
        {RANK1, {SUIT1, SUIT2, SUIT3}}, {RANK2, {SUIT1, SUIT3}}, {RANK3, {}}};
    auto cards = RankMapToVector(cards_map);
    const Storage storage(cards);
    ASSERT_EQ(Convert(cards_map[RANK1]), storage.GetSuitsForRank(RANK1));
    ASSERT_EQ(Convert(cards_map[RANK2]), storage.GetSuitsForRank(RANK2));
    ASSERT_TRUE(storage.GetSuitsForRank(RANK3).empty());
}

TEST(Cards_Storage, HasCard) {
    static const std::vector<Card> cards{{0, 1}, {1, 10}};
    const Storage storage(cards);
    ASSERT_TRUE(storage.HasCard(cards[0]));
    ASSERT_TRUE(storage.HasCard(cards[1]));
    ASSERT_FALSE(storage.HasCard({99, 99}));
}

TEST(Cards_Storage, TakeCard) {
    static const std::vector<Card> cards{{0, 1}, {1, 10}};
    Storage storage(cards);
    ASSERT_TRUE(storage.HasCard(cards[0]));
    ASSERT_TRUE(storage.HasCard(cards[1]));

    storage.TakeCard(cards[0]);
    ASSERT_FALSE(storage.HasCard(cards[0]));
    ASSERT_TRUE(storage.HasCard(cards[1]));

    storage.TakeCard(cards[1]);
    ASSERT_TRUE(storage.GetAllAvailableCards().empty());
}
