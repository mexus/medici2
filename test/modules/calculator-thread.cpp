#include "calculator-thread.h"
#include <calculator/thread.h>
#include "operators.h"

logxx::Log TestCalculatorThread::cLog("TestCalculatorThread");
MixersFactory TestCalculatorThread::mixersFactory;

using namespace calculator;

TestCalculatorThread::TestCalculatorThread() : TestFW("calculator-thread")
{
}

bool TestCalculatorThread::Tests()
{
    S_LOG("Tests");
    try {
        return TestRunning() && TestRunningMultithreaded();
    } catch (const std::exception& e) {
        log(logxx::error) << "Caught an exception: " << e.what() << logxx::endl;
        return false;
    }
}

bool TestCalculatorThread::TestRunning()
{
    S_LOG("TestRunning");
    DeckSelectors deckSelector;
    auto patienceSelector = TestCalculatorThread::DefaultPatienceSelector();

    Thread thread(deckSelector, patienceSelector, mixersFactory.CreateMixer<Card, N>(0));
    thread.Launch();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto params = thread.GetExecutionParameters();
    log(logxx::debug) << "Parameters: " << params << logxx::endl;

    return params.checkedDecks != 0 && params.suitableDecks != 0;
}

bool TestCalculatorThread::TestRunningMultithreaded()
{
    S_LOG("TestRunningMultithreaded");
    DeckSelectors deckSelector;
    auto patienceSelector = TestCalculatorThread::DefaultPatienceSelector();

    static const std::size_t testThreadsCount = 4;
    std::vector<std::unique_ptr<Thread>> threads;
    for (std::size_t i = 0; i != testThreadsCount; ++i) {
        threads.emplace_back(new Thread(deckSelector, patienceSelector, mixersFactory.CreateMixer<Card, N>(0)));
        threads.back()->Launch();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (auto & thread : threads) {
        auto params = thread->GetExecutionParameters();
        log(logxx::debug) << "Parameters: " << params << logxx::endl;
        if (params.checkedDecks == 0 || params.suitableDecks == 0)
            return false;
    }
    return true;
}


medici::PPatienceSelector TestCalculatorThread::DefaultPatienceSelector()
{
    return medici::PPatienceSelector(new medici::PatienceSelector());
}

