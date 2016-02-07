#ifndef CALCULATOR_THREAD_H
#define CALCULATOR_THREAD_H

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <cards/standard-36-deck.h>
#include <cards/deck-selector.h>
#include <medici/patience-selector.h>
#include <mixer/factory.h>

namespace calculator {

struct ExecutionParameters {
    std::uintmax_t checkedDecks, suitableDecks;
    std::chrono::steady_clock::duration runningTime;
};

class BaseThread {
public:
    BaseThread(const DeckSelectors&, const medici::PPatienceSelector&);
    virtual ~BaseThread() = default;  // In a child's class destructor one
                                      // should call JoinThread();

    void Launch();
    ExecutionParameters GetExecutionParameters() const;

protected:
    virtual void Run() = 0;
    void JoinThread();

    const DeckSelectors& deckSelector;
    const medici::PPatienceSelector& patienceSelector;

    std::unique_ptr<std::thread> thread;

    std::atomic_bool localInterrupt;
    std::atomic_bool inSchedule;
    std::mutex threadStartedMutex;
    std::condition_variable threadStarted;

    std::atomic_uintmax_t checkedDecks, suitableDecks;
    std::chrono::steady_clock::time_point launchedAt;

    std::mutex accessDecks;
};

class Thread : public BaseThread {
public:
    typedef std::unique_ptr<::MixerInterface<Card>> MixerInterface;

    typedef std::pair<std::vector<Card>, medici::PatienceInfo> FoundType;
    typedef std::vector<FoundType> FoundVector;

    Thread(const DeckSelectors& deckSelector,
           const medici::PPatienceSelector& patienceSelector, MixerInterface&& mixer)
            : BaseThread(deckSelector, patienceSelector), mixer(std::move(mixer)) {}

    ~Thread() { JoinThread(); }

    FoundVector GetNewDecks() {
        std::lock_guard<std::mutex> guard(accessDecks);
        return std::move(foundDecks);
    }

private:
    MixerInterface mixer;
    FoundVector foundDecks;

    void Run() override {
        launchedAt = std::chrono::steady_clock::now();
        inSchedule = false;
        threadStarted.notify_one();

        auto deck = standard_36_deck::kDeck;
        medici::PatienceInfo patienceInfo;
        while (!localInterrupt) {
            mixer->Mix(deck);
            ++checkedDecks;

            if (deckSelector.Check(deck) && medici::TryToConverge(deck, patienceInfo) &&
                patienceSelector->Check(deck, patienceInfo)) {
                ++suitableDecks;
                std::lock_guard<std::mutex> guard(accessDecks);
                foundDecks.push_back({deck, patienceInfo});
            }
        }
    }
};
}

#endif /* CALCULATOR_THREAD_H */
