#ifndef CARDS_DECK_SELECTOR_HPP
#define CARDS_DECK_SELECTOR_HPP

#include "deck-selector.h"

template<std::size_t N>
bool DeckAbstractSelector::Check(const std::array<Card, N>& deck) const{
	std::vector<Card> deckPart(deck.begin() + from, deck.begin() + to + 1);
	return Check(deckPart);
}

template<std::size_t N>
bool DeckSelectors::Check(const std::array<Card, N>& deck) const{
	for (auto &deckSelector : deckSelectors){
		if (!deckSelector->Check(deck))
			return false;
	}
	return true;
}

#endif /* CARDS_DECK_SELECTOR_HPP */
