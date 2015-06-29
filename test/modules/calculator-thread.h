#ifndef TEST_CALCULATOR_THREAD_H
#define TEST_CALCULATOR_THREAD_H

#include <test_fw.h>
#include <calculator/thread.h>

class TestCalculatorThread : public TestFW {
public:
    TestCalculatorThread();

    static medici::PPatienceSelector DefaultPatienceSelector();
protected:
    static constexpr std::size_t N = 36;
    static logxx::Log cLog;
    static MixersFactory mixersFactory;

    bool Tests() override;

    static bool TestRunning();
    static bool TestRunningMultithreaded();
};

#endif /* TEST_CALCULATOR_THREAD_H */
