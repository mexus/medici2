#pragma once
#include <cards/deck-selector.h>

class AceSelector : public DeckAbstractSelector {
public:
    AceSelector();

    bool Check(const std::vector<Card>& deck) const override;
    static bool IsAce(const Card& card);
};
