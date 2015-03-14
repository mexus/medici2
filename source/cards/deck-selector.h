#ifndef CARDS_DECK_SELECTOR_H
#define CARDS_DECK_SELECTOR_H

#include "card-selector.h"
#include <vector>
#include <memory>

class DeckAbstractSelector{
public:
    template<std::size_t N>
    bool Check(const std::array<Card, N>& deck) const;

protected:
    const std::vector<CardSelector> cardSelectors;
    const std::size_t from, to;

    DeckAbstractSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
    DeckAbstractSelector(std::vector<CardSelector>&&, std::size_t from, std::size_t to);
    virtual bool Check(const std::vector<Card>& deckPart) const = 0;
};


class DeckSelectors{
public:
    template<class T>
    typename std::enable_if<std::is_base_of<DeckAbstractSelector, T>::value, void>::type
    AddDeckSelector(const T&);

    void AddDeckSelector(std::unique_ptr<DeckAbstractSelector>&&);

    template<std::size_t N>
    bool Check(const std::array<Card, N>& deck) const;

    template<std::size_t N>
    bool operator()(const std::array<Card, N>& deck) const {
        return Check(deck);
    }

    DeckSelectors() = default;
    DeckSelectors(const DeckSelectors&) = delete;
    DeckSelectors(DeckSelectors&&) = default;
    DeckSelectors& operator=(DeckSelectors&&) = default;

    bool IsEmpty() const;
private:
    std::vector<std::unique_ptr<DeckAbstractSelector>> deckSelectors;
};

class DeckAllSelector : public DeckAbstractSelector {
public:
    DeckAllSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
    DeckAllSelector(std::vector<CardSelector>&&, std::size_t from, std::size_t to);
protected:
    bool Check(const std::vector<Card>& deckPart) const;
};

class DeckOneSelector : public DeckAbstractSelector {
public:
    DeckOneSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
    DeckOneSelector(std::vector<CardSelector>&&, std::size_t from, std::size_t to);
protected:
    bool Check(const std::vector<Card>& deckPart) const;
};

#include "deck-selector.hpp"

#endif /* CARDS_DECK_SELECTOR_H */
