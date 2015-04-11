#ifndef TEST_FW_H
#define TEST_FW_H

#include <logxx/logxx.h>
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

private:
    logxx::LogLevel desiredLevel;

};

#endif /* TEST_FW_H */

