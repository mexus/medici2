#pragma once

#include <cstdint>

struct Card {
    std::uint_fast8_t suit;
    std::uint_fast8_t rank;
};

bool operator<(const Card& lhs, const Card& rhs);
bool operator==(const Card& lhs, const Card& rhs);
bool operator!=(const Card& lhs, const Card& rhs);
