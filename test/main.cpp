#include <vector>
#include <map>

#include "test_fw.h"
#include <logxx/logxx.h>
#include <stdbool.h>

#include "modules/mixer.h"
#include "modules/medici.h"
#include "modules/deck-selector.h"
#include "modules/i-ching.h"

logxx::Log cLog("testing");

std::map<std::shared_ptr<TestFW>, bool> tests;

template<class T, class ...Args>
void AddTest(bool defaultVal, Args&& ...args){
        static_assert(std::is_base_of<TestFW, T>::value, "T should be derived from TestFW");
        tests[std::make_shared<T>(args...)] = defaultVal;
}

void SetAll(bool val){
        for (auto &pair : tests){
                pair.second = val;
        }
}

void Set(const std::string& arg){
        S_LOG("Set");
        if (!arg.empty()){
                bool val;
                std::string label;
                if (arg[0] == '-'){
                        label = arg.substr(1);
                        val = false;
                } else{
                        label = arg;
                        val = true;
                }
                if (!label.empty()){
                        for (auto &pair : tests){
                                if (pair.first->GetLabel() == label){
                                        pair.second = val;
                                        return ;
                                }
                        }
                        log(logxx::warning) << "No test {" << label << "} found" << logxx::endl;
                }
        }
}

int main(int argc, char **argv) {
        S_LOG("main");
        logxx::GlobalLogLevel(logxx::warning);
        bool res(true);
	bool colouredOutput(true);
        
	AddTest<TestMixer>(false);
	AddTest<TestMedici>(false);
	AddTest<TestDeckSelector>(false);
	AddTest<TestIChing>(true);
        
        for (int i = 1; i < argc; ++i){
                std::string arg(argv[i]);
                if (arg == "all")
                        SetAll(true);
                else if (arg == "none")
                        SetAll(false);
                else
                        Set(arg);
        }
        
        log(logxx::info) << "Starting tests" << logxx::endl;
        
        std::vector<std::string> passed, failed;
        
        for (auto &pair : tests){
                if (pair.second){
                        std::string label = pair.first->GetLabel();
                        bool testRes = pair.first->RunTests(colouredOutput);
                        if (testRes)
                                passed.push_back(label);
                        else
                                failed.push_back(label);
                        res &= testRes;
                }
        }
        
	if (colouredOutput){
		if (!passed.empty())
			log(logxx::info) << "\033[1;32mPASSED\033[0m tests: " << passed << logxx::endl;
		else
			log(logxx::info) << "\033[0;31mNo tests passed\033[0m" << logxx::endl;

		if (!failed.empty())
			log(logxx::info) << "\033[1;31mFAILED\033[0m tests: " << failed << logxx::endl;
		else
			log(logxx::info) << "\033[0;36mNo tests failed\033[0m" << logxx::endl;
	} else {
		if (!passed.empty())
			log(logxx::info) << "PASSED tests: " << passed << logxx::endl;
		else
			log(logxx::info) << "No tests passed" << logxx::endl;

		if (!failed.empty())
			log(logxx::info) << "FAILED tests: " << failed << logxx::endl;
		else
			log(logxx::info) << "No tests failed" << logxx::endl;
	}
        
        return res ? 0 : 1;
}

