#pragma once

#include "simple-sequence.h"

namespace cards {

class Storage;

class Sequence {
public:
    Sequence();
    Sequence(const Sequence& other);
    Sequence(Sequence&& other);
    Sequence(SimpleSequence&& sequence);

    Sequence& operator=(Sequence&& other);

    Sequence ShiftLeft(size_t shift) const;
    Sequence ShiftRight(size_t shift) const;
    bool Contradicts(const Sequence& other) const;
    bool Contradicts(const SimpleSequence& other) const;

    void AddPart(const SimpleSequence& part);
    bool IsOccupied(size_t place_number) const;
    Card GetCard(size_t place_number) const;
    size_t OccupiedPlaces() const;

    size_t GetFirstOccupied() const;
    size_t GetLastOccupied() const;

    void TakeCardsFromStorage(Storage* storage) const;
    const std::vector<SimpleSequence>& GetParts() const;

    std::vector<Sequence> operator+(const std::vector<Sequence>& sequences) const;

private:
    std::vector<SimpleSequence> parts_;

    void AddSequence(const Sequence& sequence);

    friend bool operator==(const Sequence& lhs, const Sequence& rhs);
};

bool operator==(const Sequence& lhs, const Sequence& rhs);
}
