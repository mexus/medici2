#pragma once

#include <cards/card.h>
#include <map>
#include <set>
#include <vector>

namespace medici {

struct PatienceInfo {
    std::map<Card, std::size_t> convolutions;
    std::set<Card> mobiles, stationars;
    void Clear();
};

bool TryToConverge(const std::vector<Card>& deck, PatienceInfo& info);
}
