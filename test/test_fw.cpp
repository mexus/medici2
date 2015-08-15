#include "test_fw.h"
#include <helpers/time_measure.h>
#include <easylogging++.h>
#include <fstream>
#include <cstring>
#include <map>

TestFW::TestFW(const std::string& label) : label(label) {
}

TestFW::~TestFW() {
}

std::string ResultToString(bool result) {
    if (result)
        return "PASSED";
    else
        return "FAILED";
}

std::string ResultToColouredString(bool result) {
    if (result)
        return "\033[1;32mPASSED\033[0m";
    else
        return "\033[1;31mFAILED\033[0m";
}

bool TestFW::RunTests(bool colouredOutput) {
    auto result_to_string = colouredOutput ? &ResultToString : &ResultToColouredString;
    LOG(INFO) << "Starting test [" << label << "]";
    TimeMeasure timeMeasure;
    bool res = Tests();
    LOG(INFO) << "Test " << result_to_string(res) << " in " << timeMeasure.Elapsed()
              << "s";
    return res;
}

std::string TestFW::GetLabel() const {
    return label;
}

bool TestFW::CompareFiles(const std::string& testName, const std::string& etalonName) {
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
                break;
            else if (!test || !etalon)
                return false;
        }
        return true;
    }
}
