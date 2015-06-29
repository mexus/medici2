#include "calculator-manager.h"
#include <calculator/manager.h>
#include "calculator-thread.h"
#include "operators.h"

logxx::Log TestCalculatorManager::cLog("TestCalculatorManager");
MixersFactory TestCalculatorManager::mixersFactory(TestCalculatorManager::N);

using namespace calculator;

TestCalculatorManager::TestCalculatorManager() : TestFW("calculator-manager")
{
}

bool TestCalculatorManager::Tests()
{
    return TestLaunch() && TestIncrease() && TestDecrease() && TestInterrupt();
}

bool TestCalculatorManager::TestLaunch()
{
    S_LOG("TestLaunch");
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector), TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount) {
        log(logxx::error) << "There should be " << testThreadsCount << " threads, but only " << allParams.size() << " detected" << logxx::endl;
        return false;
    }

    for (auto& params : allParams) {
        log(logxx::debug) << "Params: " << params << logxx::endl;
        if (params.checkedDecks == 0 || params.suitableDecks == 0) {
            log(logxx::error) << "No threads has started" << logxx::endl;
            return false;
        }
    }

    return true;
}

bool TestCalculatorManager::TestIncrease()
{
    S_LOG("TestIncrease");
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector), TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.IncreaseThreads();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount + 1) {
        log(logxx::error) << "There should be " << testThreadsCount + 1 << " threads, but only " << allParams.size() << " detected" << logxx::endl;
        return false;
    }

    for (auto& params : allParams) {
        log(logxx::debug) << "Params: " << params << logxx::endl;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }

    return true;
}

bool TestCalculatorManager::TestDecrease()
{
    S_LOG("TestDecrease");
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector), TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.DecreaseThreads();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount - 1) {
        log(logxx::error) << "There should be " << testThreadsCount - 1 << " threads, but only " << allParams.size() << " detected" << logxx::endl;
        return false;
    }

    for (auto& params : allParams) {
        log(logxx::debug) << "Params: " << params << logxx::endl;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }

    return true;
}

bool TestCalculatorManager::TestInterrupt()
{
    S_LOG("TestInterrupt");
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector), TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.Interrupt();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (!allParams.empty()) {
        log(logxx::error) << "There should be no threads, but " << allParams.size() << " detected" << logxx::endl;
        return false;
    }
    return true;
}

