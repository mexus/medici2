#ifndef TEST_DECK_SELECTOR
#define TEST_DECK_SELECTOR

#define protected public
#include <cards/deck-selector.h>
#undef protected
#include <test_fw.h>
#include <cards/standard-36-deck.h>

class TestDeckSelector : public TestFW{
public:
    TestDeckSelector();
protected:
    static logxx::Log cLog;
    static CardSelectorConfigurator configurator;
    bool Tests();

    bool TestAllSelector();
    bool TestOneSelector();
    bool TestComplex();

    static bool TestSelector(const std::vector<Card>& deck, const DeckAbstractSelector&, bool etalonResult);
    static CardSelector SelectorAnyRank(std::uint_fast8_t suit, bool straight = true);
    static CardSelector SelectorAnySuit(std::uint_fast8_t rank, bool straight = true);
    static CardSelector Selector(std::uint_fast8_t suit, std::uint_fast8_t rank, bool straight = true);
};

#endif /* TEST_DECK_SELECTOR */

