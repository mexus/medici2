#include <vector>
#include <map>

#include "test_fw.h"
#include "operators.h"
#include <stdbool.h>
#include <easylogging++.h>

#include "modules/mixer.h"
#include "modules/medici.h"
#include "modules/deck-selector.h"
#include "modules/i-ching.h"
#include "modules/calculator-thread.h"
#include "modules/calculator-manager.h"

INITIALIZE_EASYLOGGINGPP

std::map<std::shared_ptr<TestFW>, bool> tests;

template <class T, class... Args>
void AddTest(bool defaultVal, Args &&... args) {
    static_assert(std::is_base_of<TestFW, T>::value, "T should be derived from TestFW");
    tests[std::make_shared<T>(args...)] = defaultVal;
}

void SetAll(bool val) {
    for (auto &pair : tests) {
        pair.second = val;
    }
}

void Set(const std::string &arg) {
    if (!arg.empty()) {
        bool val;
        std::string label;
        if (arg[0] == '-') {
            label = arg.substr(1);
            val = false;
        } else {
            label = arg;
            val = true;
        }
        if (!label.empty()) {
            for (auto &pair : tests) {
                if (pair.first->GetLabel() == label) {
                    pair.second = val;
                    return;
                }
            }
            LOG(INFO) << "No test {" << label << "} found";
        }
    }
}

int main(int argc, char **argv) {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::Format,
                    "%level [%fbase:%line] %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    bool res(true);
    bool colouredOutput(true);

    AddTest<TestMixer>(false);
    AddTest<TestMedici>(false);
    AddTest<TestDeckSelector>(false);
    AddTest<TestIChing>(false);
    AddTest<TestCalculatorThread>(false);
    AddTest<TestCalculatorManager>(true);

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "all")
            SetAll(true);
        else if (arg == "none")
            SetAll(false);
        else
            Set(arg);
    }

    LOG(INFO) << "Starting tests";

    std::vector<std::string> passed, failed;

    for (auto &pair : tests) {
        if (pair.second) {
            std::string label = pair.first->GetLabel();
            bool testRes = pair.first->RunTests(colouredOutput);
            if (testRes)
                passed.push_back(label);
            else
                failed.push_back(label);
            res &= testRes;
        }
    }

    if (colouredOutput) {
        if (!passed.empty())
            LOG(INFO) << "\033[1;32mPASSED\033[0m tests: " << passed;
        else
            LOG(INFO) << "\033[0;31mNo tests passed\033[0m";
        if (!failed.empty())
            LOG(INFO) << "\033[1;31mFAILED\033[0m tests: " << failed;
        else
            LOG(INFO) << "\033[0;36mNo tests failed\033[0m";
    } else {
        if (!passed.empty())
            LOG(INFO) << "PASSED tests: " << passed;
        else
            LOG(INFO) << "No tests passed";
        if (!failed.empty())
            LOG(INFO) << "FAILED tests: " << failed;
        else
            LOG(INFO) << "No tests failed";
    }

    return res ? 0 : 1;
}
