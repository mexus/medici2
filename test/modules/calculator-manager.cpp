#include "calculator-manager.h"
#include <calculator/manager.h>
#include "calculator-thread.h"
#include "operators.h"
#include <easylogging++.h>

MixersFactory TestCalculatorManager::mixersFactory(TestCalculatorManager::N);

using namespace calculator;

TestCalculatorManager::TestCalculatorManager() : TestFW("calculator-manager") {
}

bool TestCalculatorManager::Tests() {
    return TestLaunch() && TestIncrease() && TestDecrease() && TestInterrupt();
}

bool TestCalculatorManager::TestLaunch() {
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector),
                   TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount) {
        LOG(ERROR) << "There should be " << testThreadsCount << " threads, but only "
                   << allParams.size() << " detected";
        return false;
    }

    for (auto& params : allParams) {
        LOG(DEBUG) << "Params: " << params;
        if (params.checkedDecks == 0 || params.suitableDecks == 0) {
            LOG(ERROR) << "No threads has started";
            return false;
        }
    }

    return true;
}

bool TestCalculatorManager::TestIncrease() {
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector),
                   TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.IncreaseThreads();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount + 1) {
        LOG(ERROR) << "There should be " << testThreadsCount + 1 << " threads, but only "
                   << allParams.size() << " detected";
        return false;
    }

    for (auto& params : allParams) {
        LOG(DEBUG) << "Params: " << params;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }

    return true;
}

bool TestCalculatorManager::TestDecrease() {
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector),
                   TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.DecreaseThreads();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (allParams.size() != testThreadsCount - 1) {
        LOG(ERROR) << "There should be " << testThreadsCount - 1 << " threads, but only "
                   << allParams.size() << " detected";
        return false;
    }

    for (auto& params : allParams) {
        LOG(DEBUG) << "Params: " << params;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }

    return true;
}

bool TestCalculatorManager::TestInterrupt() {
    DeckSelectors deckSelector;

    Manager manager(mixersFactory);
    static const std::size_t testThreadsCount = 2;
    manager.Launch(testThreadsCount, std::move(deckSelector),
                   TestCalculatorThread::DefaultPatienceSelector());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.Interrupt();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto allParams = manager.GetExecutionParameters();
    if (!allParams.empty()) {
        LOG(ERROR) << "There should be no threads, but " << allParams.size()
                   << " detected";
        return false;
    }
    return true;
}
