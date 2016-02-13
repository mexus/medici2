#pragma once

#include <cards/card.h>
#include <map>
#include <set>
#include <vector>

namespace medici {

struct PatienceInfo {
    std::map<cards::Card, std::size_t> convolutions;
    std::set<cards::Card> mobiles, stationars;
    void Clear();
};

bool TryToConverge(const std::vector<cards::Card>& deck, PatienceInfo& info);
}
