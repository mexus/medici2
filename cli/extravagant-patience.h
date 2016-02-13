#pragma once

#include <medici/patience-selector.h>

class ExtravagantPatience : public medici::PatienceSelector {
public:
    bool Check(const std::vector<cards::Card>& deck,
               const medici::PatienceInfo& info) override;

private:
    static bool IsAce(const cards::Card& card);
    static size_t GetConvolutions(const std::map<cards::Card, size_t>& convolutions,
                                  const cards::Card& card);
};
