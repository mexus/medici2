#include "thread.h"
#include <iostream>

namespace calculator {

    BaseThread::BaseThread(const DeckSelectors& deckSelectors, const medici::PPatienceSelector& patienceSelector) :
        deckSelector(deckSelectors), patienceSelector(patienceSelector),
        localInterrupt(false), inSchedule(false), checkedDecks(0), suitableDecks(0)
    {
    }

    void BaseThread::JoinThread()
    {
        localInterrupt = true;
        if (inSchedule) {
            std::unique_lock<std::mutex> guard(threadStartedMutex);
            threadStarted.wait(guard, [this]{return !inSchedule;});
        }
        if (thread && thread->joinable())
            thread->join();
    }

    void BaseThread::Launch()
    {
        if (!thread && !localInterrupt) {
            inSchedule = true;
            thread.reset(new std::thread(&BaseThread::Run, this));
        }
    }

    ExecutionParameters BaseThread::GetExecutionParameters() const
    {
        return {checkedDecks, suitableDecks, std::chrono::steady_clock::now() - launchedAt};
    }

}

