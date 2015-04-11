#ifndef CARDS_DECK_SELECTOR_H
#define CARDS_DECK_SELECTOR_H

#include "card-selector.h"
#include <vector>
#include <memory>

class DeckAbstractSelector{
public:
    template<std::size_t N>
    bool Check(const std::array<Card, N>& deck) const
    {
        std::vector<Card> deckPart(deck.begin() + from, deck.begin() + to + 1);
        return Check(deckPart);
    }

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
    AddDeckSelector(const T& s)
    {
        deckSelectors.emplace_back(new T(s));
    }

    void AddDeckSelector(std::unique_ptr<DeckAbstractSelector>&&);

    template<std::size_t N>
    bool Check(const std::array<Card, N>& deck) const
    {
        for (auto &deckSelector : deckSelectors) {
            if (!deckSelector->Check(deck))
                return false;
        }
        return true;
    }

    template<std::size_t N>
    bool operator()(const std::array<Card, N>& deck) const
    {
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

#endif /* CARDS_DECK_SELECTOR_H */
