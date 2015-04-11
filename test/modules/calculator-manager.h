#ifndef TEST_CALCULATOR_MANAGER_H
#define TEST_CALCULATOR_MANAGER_H

#include <test_fw.h>

class TestCalculatorManager : public TestFW {
public:
    TestCalculatorManager();
protected:
    static logxx::Log cLog;

    bool Tests() override;

    static bool TestLaunch();
    static bool TestIncrease();
    static bool TestDecrease();
    static bool TestInterrupt();

};

#endif /* TEST_CALCULATOR_MANAGER_H */
