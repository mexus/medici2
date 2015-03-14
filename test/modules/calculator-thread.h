#ifndef TEST_CALCULATOR_THREAD_H
#define TEST_CALCULATOR_THREAD_H

#include "../test_fw.h"

class TestCalculatorThread : public TestFW {
public:
    TestCalculatorThread();
protected:
    static logxx::Log cLog;

    bool Tests() override;

    static bool TestRunning();
    static bool TestRunningMultithreaded();
};

#endif /* TEST_CALCULATOR_THREAD_H */
