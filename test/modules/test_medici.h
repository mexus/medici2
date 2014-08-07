#ifndef TEST_MEDICI
#define TEST_MEDICI

#include <medici/medici.h>
#include <cards/standard-36-deck.h>
#include "../test_fw.h"

class TestMedici : public TestFW{
public:
	typedef standard_36_deck::Deck DeckType;
	typedef standard_36_deck::Deck::ArrayType ArrayType;

	TestMedici();
protected:
	static logxx::Log cLog;
	bool Tests() ;
	bool Test(const ArrayType& deck, bool converges, const Medici::PatienceInfo& etalonInfo);

	bool Compare(const Medici::PatienceInfo& result, const Medici::PatienceInfo& etalon);
};

#endif /* TEST_MEDICI */
