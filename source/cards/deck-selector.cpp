#include "deck-selector.h"

typedef std::vector<CardSelector> CardSelectors;

DeckAbstractSelector::DeckAbstractSelector(const CardSelectors& selectors, std::size_t from, std::size_t to) :
    cardSelectors(selectors), from(from), to(to)
{
}

DeckAbstractSelector::DeckAbstractSelector(CardSelectors&& selectors, std::size_t from, std::size_t to) :
    cardSelectors(selectors), from(from), to(to)
{
}

void DeckSelectors::AddDeckSelector(std::unique_ptr<DeckAbstractSelector>&& selector)
{
    deckSelectors.push_back(std::move(selector));
}

bool DeckSelectors::IsEmpty() const {
    return deckSelectors.empty();
}

DeckAllSelector::DeckAllSelector(const CardSelectors& selectors, std::size_t from, std::size_t to) :
    DeckAbstractSelector(selectors, from, to)
{
}

DeckAllSelector::DeckAllSelector(CardSelectors&& selectors, std::size_t from, std::size_t to) :
    DeckAbstractSelector(selectors, from, to)
{
}

bool DeckAllSelector::CheckPart(const std::vector<Card>& deckPart) const
{
    for (auto &selector : cardSelectors) {
        for (auto &card : deckPart) {
            if (!selector.Check(card))
                return false;
        }
    }
    return true;
}


DeckOneSelector::DeckOneSelector(const CardSelectors& selectors, std::size_t from, std::size_t to) :
    DeckAbstractSelector(selectors, from, to)
{
}

DeckOneSelector::DeckOneSelector(CardSelectors&& selectors, std::size_t from, std::size_t to) :
    DeckAbstractSelector(selectors, from, to)
{
}

bool DeckOneSelector::CheckPart(const std::vector<Card>& deckPart) const
{
    for (auto &selector : cardSelectors) {
        for (auto &card : deckPart) {
            if (selector.Check(card))
                return true;
        }
    }
    return false;
}

