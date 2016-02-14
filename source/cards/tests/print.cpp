#include "print.h"

#include <gtest/gtest.h>

namespace cards {

void PrintTo(const SimpleSequence& sequence, ::std::ostream* s) {
    *s << "{at " << sequence.position << ", " << sequence.cards.size() << ":";
    for (auto& card : sequence.cards) {
        *s << " [" << (int)card.suit << ", " << (int)card.rank << "]";
    }
    *s << "}";
}

void PrintTo(const Sequence& sequence, ::std::ostream* s) {
    *s << ::testing::PrintToString(sequence.GetParts());
}
}
