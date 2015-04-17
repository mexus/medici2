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

    class Thread {
    public:
        typedef standard_36_deck::Deck StandardDeck;
        typedef StandardDeck::ArrayType StandardDeckArray;
        typedef std::unique_ptr<MixerInterface<Card, StandardDeck::N()>> StandardMixer;

        typedef std::pair<StandardDeckArray, medici::Patience::PatienceInfo> FoundType;
        typedef std::vector<FoundType> FoundVector;

        struct RunParameters {
            std::uintmax_t checkedDecks, suitableDecks;
            std::chrono::steady_clock::duration runningTime;
        };

        Thread(const DeckSelectors&, const medici::PPatienceSelector&, StandardMixer&& mixer);
        ~Thread();

        void Launch();
        RunParameters GetRunParameters() const;
        FoundVector GetNewDecks();

    private:
        const DeckSelectors& deckSelector;
        const medici::PPatienceSelector& patienceSelector;
        StandardMixer mixer;

        std::atomic_bool localInterrupt;
        std::atomic_bool inSchedule;

        std::mutex threadStartedMutex;
        std::condition_variable threadStarted;

        std::atomic_uintmax_t checkedDecks, suitableDecks;
        std::thread* thread;

        std::chrono::steady_clock::time_point launchedAt;

        FoundVector foundDecks;
        std::mutex accessDecks;
        std::size_t givenAwayDecks = 0;

        void Run();
    };

}

#endif /* CALCULATOR_THREAD_H */
