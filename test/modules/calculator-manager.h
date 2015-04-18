#ifndef TEST_CALCULATOR_MANAGER_H
#define TEST_CALCULATOR_MANAGER_H

#include <test_fw.h>

#include <mixer/factory.h>

class TestCalculatorManager : public TestFW {
public:
    TestCalculatorManager();
protected:
    static constexpr std::size_t N = 36;

    static logxx::Log cLog;
    static MixersFactory mixersFactory;

    bool Tests() override;

    static bool TestLaunch();
    static bool TestIncrease();
    static bool TestDecrease();
    static bool TestInterrupt();
};

#endif /* TEST_CALCULATOR_MANAGER_H */
