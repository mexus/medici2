#ifndef CARDS_DECK_SELECTOR_HPP
#define CARDS_DECK_SELECTOR_HPP

#include "deck-selector.h"

template<std::size_t N>
bool DeckAbstractSelector::Check(const std::array<Card, N>& deck) const{
	std::vector<Card> deckPart(deck.begin() + from, deck.begin() + to + 1);
	return Check(deckPart);
}

template<class T>
typename std::enable_if<std::is_base_of<DeckAbstractSelector, T>::value, void>::type DeckSelectors::AddDeckSelector(const T& s){
	deckSelectors.emplace_back(new T(s));
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
