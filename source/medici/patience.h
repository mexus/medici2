#ifndef MEDICI_PATIENCE_H
#define MEDICI_PATIENCE_H

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

bool TryToConverge(const std::vector<Card>& deck, PatienceInfo&);
std::size_t ConvergeDeckPart(std::vector<Card>& deck, PatienceInfo&);
void PopulateMobiles(const Card& leftCard, const Card& middleCard, PatienceInfo& info);
bool CheckConvergence(const Card& left, const Card& right);
}

#endif /* MEDICI_PATIENCE_H */
