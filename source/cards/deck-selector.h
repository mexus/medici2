#ifndef CARDS_DECK_SELECTOR_H
#define CARDS_DECK_SELECTOR_H

#include "card-selector.h"
#include <vector>
#include <memory>

class DeckAbstractSelector{
public:
    bool Check(const std::vector<Card>& deck) const
    {
        std::vector<Card> deckPart(deck.begin() + from, deck.begin() + to + 1);
        return CheckPart(deckPart);
    }

protected:
    const std::vector<CardSelector> cardSelectors;
    const std::size_t from, to;

    DeckAbstractSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
    DeckAbstractSelector(std::vector<CardSelector>&&, std::size_t from, std::size_t to);
    virtual bool CheckPart(const std::vector<Card>& deckPart) const = 0;
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

    bool Check(const std::vector<Card>& deck) const
    {
        for (auto &deckSelector : deckSelectors) {
            if (!deckSelector->Check(deck))
                return false;
        }
        return true;
    }

    bool operator()(const std::vector<Card>& deck) const
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
    bool CheckPart(const std::vector<Card>& deckPart) const override;
};

class DeckOneSelector : public DeckAbstractSelector {
public:
    DeckOneSelector(const std::vector<CardSelector>&, std::size_t from, std::size_t to);
    DeckOneSelector(std::vector<CardSelector>&&, std::size_t from, std::size_t to);
protected:
    bool CheckPart(const std::vector<Card>& deckPart) const override;
};

#endif /* CARDS_DECK_SELECTOR_H */
