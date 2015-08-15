#include "calculator-thread.h"
#include <calculator/thread.h>
#include "operators.h"
#include <easylogging++.h>

MixersFactory TestCalculatorThread::mixersFactory(TestCalculatorThread::N);

using namespace calculator;

TestCalculatorThread::TestCalculatorThread() : TestFW("calculator-thread") {
}

bool TestCalculatorThread::Tests() {
    try {
        return TestRunning() && TestRunningMultithreaded();
    } catch (const std::exception& e) {
        LOG(ERROR) << "Caught an exception: " << e.what();
        return false;
    }
}

bool TestCalculatorThread::TestRunning() {
    DeckSelectors deckSelector;
    auto patienceSelector = TestCalculatorThread::DefaultPatienceSelector();

    Thread thread(deckSelector, patienceSelector, mixersFactory.CreateMixer<Card>(0));
    thread.Launch();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto params = thread.GetExecutionParameters();
    LOG(DEBUG) << "Parameters: " << params;

    return params.checkedDecks != 0 && params.suitableDecks != 0;
}

bool TestCalculatorThread::TestRunningMultithreaded() {
    DeckSelectors deckSelector;
    auto patienceSelector = TestCalculatorThread::DefaultPatienceSelector();

    static const std::size_t testThreadsCount = 4;
    std::vector<std::unique_ptr<Thread>> threads;
    for (std::size_t i = 0; i != testThreadsCount; ++i) {
        threads.emplace_back(new Thread(deckSelector, patienceSelector,
                                        mixersFactory.CreateMixer<Card>(0)));
        threads.back()->Launch();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (auto& thread : threads) {
        auto params = thread->GetExecutionParameters();
        LOG(DEBUG) << "Parameters: " << params;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }
    return true;
}

medici::PPatienceSelector TestCalculatorThread::DefaultPatienceSelector() {
    return medici::PPatienceSelector(new medici::PatienceSelector());
}
