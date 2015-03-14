#ifndef TEST_DECK_SELECTOR
#define TEST_DECK_SELECTOR

#define protected public
#include <cards/deck-selector.h>
#undef protected
#include "../test_fw.h"
#include <cards/standard-36-deck.h>

class TestDeckSelector : public TestFW{
public:
    typedef standard_36_deck::Deck DeckType;
    typedef standard_36_deck::Deck::ArrayType ArrayType;

    TestDeckSelector();
protected:
    static logxx::Log cLog;
    bool Tests();

    bool TestAllSelector();
    bool TestOneSelector();
    bool TestComplex();

    static bool TestSelector(const ArrayType& deck, const DeckAbstractSelector&, bool etalonResult);
};

#endif /* TEST_DECK_SELECTOR */

