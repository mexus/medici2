#include "test_fw.h"
#include <helpers/time_measure.h>
#include <fstream>
#include <cstring>

TestFW::TestFW(const std::string& label, logxx::LogLevel l) :
    label(label), desiredLevel(l)
{

}

TestFW::~TestFW()
{
}

bool TestFW::RunTests(bool colouredOutput)
{
    static const std::map<bool, std::string> colouredResult {
        {true, "\033[1;32mPASSED\033[0m"},
        {false, "\033[1;31mFAILED\033[0m"}
    };
    static const std::map<bool, std::string> nonColouredResult {
        {true, "PASSED"},
        {false, "FAILED"}
    };
    
        static logxx::Log log("TestFw::RunTests");
        log(logxx::info, label) << "Starting test" << logxx::endl;
        TimeMeasure timeMeasure;
        
        auto storedLevel = logxx::GlobalLogLevel();
        if (storedLevel != desiredLevel)
                logxx::GlobalLogLevel(desiredLevel);
        bool res = Tests();
        logxx::GlobalLogLevel(storedLevel);
        
        auto &s = log(logxx::info, label) << "Test ";
        if (colouredOutput)
                s << colouredResult.at(res);
        else
                s << nonColouredResult.at(res);
        s << " in " << timeMeasure.Elapsed() << "s" << logxx::endl;
        return res;
}

std::string TestFW::GetLabel() const {
        return label;
}

bool TestFW::CompareFiles(const std::string& testName, const std::string& etalonName)
{
    std::ifstream test(testName), etalon(etalonName);
    if (!test || !etalon)
        return false;
    else {
        while (true) {
            static const std::size_t chunk = 1024;
            char testData[chunk] = {0};
            char etalonData[chunk] = {0};
            test.read(testData, chunk);
            etalon.read(etalonData, chunk);
            if (std::strncmp(testData, etalonData, chunk) != 0)
                return false;

            if (!test && !etalon)
                break ;
            else if (!test || !etalon)
                return false;
        }
        return true;
    }
}

