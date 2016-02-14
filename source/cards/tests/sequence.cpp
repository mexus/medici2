#include <map>

#include <gtest/gtest.h>

#include <cards/sequence.h>
#include <cards/storage.h>

#include "print.h"

using namespace cards;

TEST(Cards_Sequence, DefaultConstructor) {
    const Sequence sequence;
    ASSERT_TRUE(sequence.GetParts().empty());
}

TEST(Cards_Sequence, SimpleSequenceConstructor) {
    SimpleSequence simple{{{0, 1}, {5, 5}}, 4};
    const Sequence sequence(SimpleSequence{simple});
    auto& parts = sequence.GetParts();
    ASSERT_EQ(1, parts.size());
    ASSERT_EQ(simple, parts[0]);
}

TEST(Cards_Sequence, MoveOperator) {
    SimpleSequence simple{{{0, 1}, {5, 5}}, 5};
    Sequence sequence(SimpleSequence{simple});
    Sequence moved_sequence(std::move(sequence));
    ASSERT_TRUE(sequence.GetParts().empty());
    auto& moved_parts = moved_sequence.GetParts();
    ASSERT_EQ(1, moved_parts.size());
    ASSERT_EQ(simple, moved_parts[0]);
}

TEST(Cards_Sequence, OccupiedPlaces) {
    Sequence sequence1({{{0, 1}, {5, 5}}, 4});
    sequence1.AddPart({{{1, 1}}, 7});

    ASSERT_EQ(3, sequence1.OccupiedPlaces());
}

TEST(Cards_Sequence, Equals) {
    const Sequence sequence1({{{0, 1}, {5, 5}}, 4});
    Sequence sequence2({{{0, 1}, {5, 5}}, 4});
    sequence2.AddPart({{{1, 1}}, 7});

    ASSERT_EQ(sequence1, sequence1);
    ASSERT_EQ(sequence2, sequence2);

    ASSERT_FALSE(sequence1 == sequence2);
    ASSERT_FALSE(sequence2 == sequence1);
}

TEST(Cards_Sequence, ShifLeft) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});
    sequence.AddPart({{{1, 1}}, 7});

    sequence = sequence.ShiftLeft(5);

    auto& parts = sequence.GetParts();
    ASSERT_EQ(2, parts.size());
    ASSERT_EQ(SimpleSequence({{{5, 5}}, 0}), parts[0]);
    ASSERT_EQ(SimpleSequence({{{1, 1}}, 2}), parts[1]);
}

TEST(Cards_Sequence, ShiftRight) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});
    sequence.AddPart({{{1, 1}}, 7});

    sequence = sequence.ShiftRight(2);

    auto& parts = sequence.GetParts();
    ASSERT_EQ(3, parts.size());
    ASSERT_EQ(SimpleSequence({{{7, 4}, {3, 8}}, 3}), parts[0]);
    ASSERT_EQ(SimpleSequence({{{0, 1}, {5, 5}}, 6}), parts[1]);
    ASSERT_EQ(SimpleSequence({{{1, 1}}, 9}), parts[2]);
}

TEST(Cards_Sequence, Contradicts) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});
    sequence.AddPart({{{1, 1}}, 7});

    ASSERT_FALSE(sequence.Contradicts(sequence));

    const Sequence contradictory({{{7, 3}, {3, 8}}, 1});
    ASSERT_TRUE(sequence.Contradicts(contradictory));

    Sequence non_contradictory(sequence);
    non_contradictory.AddPart({{{1, 2}}, 8});
    ASSERT_FALSE(sequence.Contradicts(non_contradictory));
}

TEST(Cards_Sequence, ContradictsSimple) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});
    sequence.AddPart({{{1, 1}}, 7});

    ASSERT_FALSE(sequence.Contradicts(SimpleSequence{{{0, 1}, {5, 5}, {6, 1}}, 4}));
    ASSERT_TRUE(sequence.Contradicts(SimpleSequence{{{1, 2}}, 7}));
    ASSERT_FALSE(sequence.Contradicts(SimpleSequence{{{3, 3}}, 0}));
    ASSERT_FALSE(sequence.Contradicts(SimpleSequence{{{81, 104}}, 3}));
    ASSERT_FALSE(sequence.Contradicts(SimpleSequence{{{2, 4}}, 8}));
}

TEST(Cards_Sequence, AddPartAssertion) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    ASSERT_DEBUG_DEATH(sequence.AddPart({{{3, 9}}, 2}), "contradict");
}

TEST(Cards_Sequence, AddPart) {
    Sequence sequence;
    const SimpleSequence part{{{0, 1}, {5, 5}}, 4};
    sequence.AddPart(part);

    auto& parts = sequence.GetParts();
    ASSERT_EQ(1, parts.size());
    ASSERT_EQ(part, parts[0]);

    sequence.AddPart({{{1, 4}}, 6});
    ASSERT_EQ(1, parts.size());
    ASSERT_EQ(SimpleSequence({{{0, 1}, {5, 5}, {1, 4}}, 4}), parts[0]);
}

TEST(Cards_Sequence, IsOccupied) {
    Sequence sequence({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});
    sequence.AddPart({{{1, 1}}, 7});

    std::set<size_t> positions{1, 2, 4, 5, 7};
    for (size_t position : positions) {
        ASSERT_TRUE(sequence.IsOccupied(position)) << "at position = " << position;
    }

    positions = std::set<size_t>{0, 3, 6, 10};
    for (size_t position : positions) {
        ASSERT_FALSE(sequence.IsOccupied(position)) << "at position = " << position;
    }
}

TEST(Cards_Sequence, GetCardAssert) {
    const Sequence sequence({{{7, 4}, {3, 8}}, 1});
    ASSERT_DEBUG_DEATH(sequence.GetCard(0), "not occupied");
}

TEST(Cards_Sequence, GetCard) {
    std::map<size_t, Card> cards{
        {0, {7, 4}}, {1, {5, 4}}, {4, {3, 2}}, {5, {10, 0}}, {10, {0, 0}}};
    Sequence sequence;
    for (auto& pair : cards) {
        size_t position = pair.first;
        const Card& card = pair.second;
        sequence.AddPart({{{card}}, position});
    }

    for (auto& pair : cards) {
        size_t position = pair.first;
        const Card& card = pair.second;

        ASSERT_EQ(card, sequence.GetCard(position)) << "at position = " << position;
    }
}

TEST(Cards_Sequence, GetFirstOccupied) {
    Sequence sequence({{{1, 1}}, 7});
    sequence.AddPart({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});

    ASSERT_EQ(1, sequence.GetFirstOccupied());
}

TEST(Cards_Sequence, GetLastOccupied) {
    Sequence sequence({{{1, 1}}, 7});
    sequence.AddPart({{{7, 4}, {3, 8}}, 1});
    sequence.AddPart({{{0, 1}, {5, 5}}, 4});

    ASSERT_EQ(7, sequence.GetLastOccupied());
}

TEST(Cards_Sequence, TakeCardsFromStorage) {
    const Sequence sequence({{{7, 4}, {3, 8}}, 1});
    Storage storage({{7, 4}, {3, 8}, {0, 0}});

    sequence.TakeCardsFromStorage(&storage);

    ASSERT_EQ(std::vector<Card>({{0, 0}}), storage.GetAllAvailableCards());
}

TEST(Cards_Sequence, AdditionOperator) {
    const Sequence sequence({{{7, 4}, {3, 8}}, 1});

    const Sequence contradictory(SimpleSequence{{{3, 9}, {4, 0}}, 2});
    const Sequence non_contradictory1(SimpleSequence(sequence.GetParts()[0]));
    const Sequence non_contradictory2(SimpleSequence{{{1, 1}, {5, 6}}, 4});
    const Sequence non_contradictory3(SimpleSequence{{{1, 1}, {5, 6}}, 3});
    const Sequence non_contradictory4(Sequence{});

    auto result = sequence + std::vector<Sequence>{contradictory, non_contradictory1,
                                                   non_contradictory2, non_contradictory3,
                                                   non_contradictory4};

    const Sequence etalon1(sequence);

    Sequence etalon2(sequence);
    etalon2.AddPart(non_contradictory2.GetParts()[0]);

    const Sequence etalon3(SimpleSequence{{{7, 4}, {3, 8}, {1, 1}, {5, 6}}, 1});
    const Sequence etalon4(sequence);

    ASSERT_EQ(4, result.size());
    ASSERT_EQ(etalon1, result[0]);
    ASSERT_EQ(etalon2, result[1]);
    ASSERT_EQ(etalon3, result[2]);
    ASSERT_EQ(etalon4, result[3]);
}
