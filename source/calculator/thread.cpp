#include "thread.h"

typedef std::lock_guard<std::mutex> Guard;

namespace calculator {
    using medici::PPatienceSelector;
    using medici::Patience;

    Thread::Thread(const DeckSelectors& deckSelector, const PPatienceSelector& patienceSelector, StandardMixer&& mixer) :
        deckSelector(deckSelector), patienceSelector(patienceSelector), mixer(std::move(mixer)), localInterrupt(false),
        inSchedule(false), checkedDecks(0), suitableDecks(0), thread(nullptr)
    {
    }

    void Thread::Launch()
    {
        if (!thread) {
            inSchedule = true;
            thread = new std::thread(&Thread::Run, this);
        }
    }

    Thread::~Thread()
    {
        if (inSchedule) {
            std::unique_lock<std::mutex> guard(threadStartedMutex);
            threadStarted.wait(guard, [this]{return !inSchedule;});
        }

        localInterrupt = true;
        if (thread && thread->joinable())
            thread->join();
        delete thread;
    }

    void Thread::Run()
    {
        launchedAt = std::chrono::steady_clock::now();
        inSchedule = false;
        threadStarted.notify_one();
        auto deck = standard_36_deck::Deck::cards;
        Patience::PatienceInfo patienceInfo;
        while (!localInterrupt) {
            mixer->Mix(deck);
            ++checkedDecks;
            if (deckSelector.Check(deck) && Patience::Converge(deck, patienceInfo) && patienceSelector->Check(patienceInfo)) {
                ++suitableDecks;
                Guard guard(accessDecks);
                foundDecks.push_back({deck, patienceInfo});
            }
        }
    }

    Thread::FoundVector Thread::GetNewDecks()
    {
        Guard guard(accessDecks);
        FoundVector result(foundDecks.begin() + givenAwayDecks, foundDecks.end());
        givenAwayDecks += result.size();
        return result;
    }

    Thread::RunParameters Thread::GetRunParameters() const {
        return {checkedDecks, suitableDecks, std::chrono::steady_clock::now() - launchedAt};
    }

}

