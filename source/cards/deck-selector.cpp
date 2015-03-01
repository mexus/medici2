#include "deck-selector.h"

DeckAbstractSelector::DeckAbstractSelector(const std::vector<CardSelector>& selectors, std::size_t from, std::size_t to) : cardSelectors(selectors), from(from), to(to){
}

DeckAbstractSelector::DeckAbstractSelector(std::vector<CardSelector>&& selectors, std::size_t from, std::size_t to) : cardSelectors(selectors), from(from), to(to){
}


DeckAllSelector::DeckAllSelector(const std::vector<CardSelector>& selectors, std::size_t from, std::size_t to) : DeckAbstractSelector(selectors, from, to) {
}

DeckAllSelector::DeckAllSelector(std::vector<CardSelector>&& selectors, std::size_t from, std::size_t to) : DeckAbstractSelector(selectors, from, to) {
}

bool DeckAllSelector::Check(const std::vector<Card>& deckPart) const{
	for (auto &selector : cardSelectors){
		for (auto &card : deckPart){
			if (!selector.Check(card))
				return false;
		}
	}
	return true;
}


DeckOneSelector::DeckOneSelector(const std::vector<CardSelector>& selectors, std::size_t from, std::size_t to) : DeckAbstractSelector(selectors, from, to) {
}

DeckOneSelector::DeckOneSelector(std::vector<CardSelector>&& selectors, std::size_t from, std::size_t to) : DeckAbstractSelector(selectors, from, to) {
}

bool DeckOneSelector::Check(const std::vector<Card>& deckPart) const{
	for (auto &selector : cardSelectors){
		for (auto &card : deckPart){
			if (selector.Check(card))
				return true;
		}
	}
	return false;
}

