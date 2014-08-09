#ifndef CARDS_DECK_SELECTOR_H
#define CARDS_DECK_SELECTOR_H

#include "card-selector.h"
#include <vector>
#include <memory>

class DeckAbstractSelector{
public:
	template<std::size_t N>
	bool Check(const std::array<Card, N>& deck) const;

	const std::vector<CardSelector> cardSelectors;
	const std::size_t from, to;
protected:
	DeckAbstractSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
	virtual bool Check(const std::vector<Card>& deckPart) const = 0;
};


class DeckSelectors{
public:
	void AddDeckSelector(const std::shared_ptr<DeckAbstractSelector>&);
	template<std::size_t N>
	bool Check(const std::array<Card, N>& deck) const;
private:
	std::vector<std::shared_ptr<DeckAbstractSelector>> deckSelectors;
};


class DeckNegateSelector : public DeckAbstractSelector {
public:
	DeckNegateSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
protected:
	bool Check(const std::vector<Card>& deckPart) const;
};

class DeckAllSelector : public DeckAbstractSelector {
public:
	DeckAllSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
protected:
	bool Check(const std::vector<Card>& deckPart) const;
};

class DeckOneSelector : public DeckAbstractSelector {
public:
	DeckOneSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
protected:
	bool Check(const std::vector<Card>& deckPart) const;
};

#include "deck-selector.hpp"

#endif /* CARDS_DECK_SELECTOR_H */
