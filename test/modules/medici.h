#ifndef TEST_MEDICI
#define TEST_MEDICI

#include <medici/patience.h>
#include <cards/standard-36-deck.h>
#include <test_fw.h>

using namespace medici;

class TestMedici : public TestFW {
public:
    TestMedici();

protected:
    static logxx::Log cLog;
    bool Tests();
    bool Test(const std::vector<Card>& deck, bool converges,
              const PatienceInfo& etalonInfo = PatienceInfo());

    bool Compare(const PatienceInfo& result, const PatienceInfo& etalon);
};

#endif /* TEST_MEDICI */
