#ifndef TEST_FW_H
#define	TEST_FW_H

#include "operators.hpp"
#include <logxx/logxx.h>
#include <vector>
#include <set>
#include <functional>

class TestFW {
public:
        TestFW(const std::string& label, logxx::LogLevel = logxx::warning);
        TestFW(const TestFW& ) = delete;
        virtual ~TestFW();
        
        bool RunTests(bool colouredOutput);
        std::string GetLabel() const;
protected:
        virtual bool Tests() = 0;
        const std::string label;

	static bool CompareFiles(const std::string&, const std::string&);

	template<class T, class Result = float>
	Result CalculateAverage(std::size_t tests, const std::function<T()>&);
private:
        logxx::LogLevel desiredLevel;

};

#include "test_fw.hpp"

#endif	/* TEST_FW_H */

