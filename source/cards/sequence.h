#pragma once

#include <cstdlib>
#include <vector>

#include "card.h"

namespace cards {

struct ClosedSequence {
    std::vector<Card> cards;
    size_t position;

    bool Contradicts(const ClosedSequence& other) const;
    bool IsOccupied(size_t place_number) const;
    Card GetCard(size_t place_number) const;
    bool Overlaps(const ClosedSequence& other) const;
    ClosedSequence Merge(const ClosedSequence& other) const;
};

bool operator==(const ClosedSequence& lhs, const ClosedSequence& rhs);

class Storage;

class Sequence {
public:
    Sequence();
    Sequence(const Sequence& other);
    Sequence(Sequence&& other);
    Sequence(ClosedSequence&& sequence);

    Sequence& operator=(Sequence&& other);

    Sequence ShiftLeft(size_t shift) const;
    Sequence ShiftRight(size_t shift) const;
    bool Contradicts(const Sequence& other) const;

    void AddPart(const ClosedSequence& part);
    void AddPart(ClosedSequence&& part);
    void AddSequence(const Sequence& sequence);
    bool IsOccupied(size_t place_number) const;
    Card GetCard(size_t place_number) const;

    size_t GetFirstOccupied() const;
    size_t GetLastOccupied() const;

    void TakeCardsFromStorage(Storage* storage) const;
    const std::vector<ClosedSequence>& GetParts() const;

    std::vector<Sequence> operator+(const std::vector<Sequence>& sequences) const;

private:
    std::vector<ClosedSequence> parts_;
    friend bool operator==(const Sequence& lhs, const Sequence& rhs);
};

bool operator==(const Sequence& lhs, const Sequence& rhs);
}
