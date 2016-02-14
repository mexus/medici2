#pragma once

#include <cstdlib>
#include <vector>

#include "card.h"

namespace cards {

struct SimpleSequence {
    std::vector<Card> cards;
    size_t position;

    bool Contradicts(const SimpleSequence& other) const;
    bool IsOccupied(size_t place_number) const;
    Card GetCard(size_t place_number) const;
    bool Overlaps(const SimpleSequence& other) const;
    SimpleSequence Merge(const SimpleSequence& other) const;
};

bool operator==(const SimpleSequence& lhs, const SimpleSequence& rhs);
}
