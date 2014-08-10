#ifndef TEST_FW_H
#define	TEST_FW_H

#include "operators.hpp"
#include <logxx/logxx.h>
#include <vector>
#include <set>
#include <map>
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

	template<class T>
	bool Compare(const std::set<T>& etalon, const std::set<T>& test);

	template<class T1, class T2>
	bool Compare(const std::map<T1, T2>& etalon, const std::map<T1, T2>& test);

	template<class T, std::size_t N>
	bool Compare(const std::array<T, N>& etalon, const std::array<T, N>& test);
private:
        logxx::LogLevel desiredLevel;

};

#include "test_fw.hpp"

#endif	/* TEST_FW_H */

