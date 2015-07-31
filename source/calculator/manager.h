#ifndef CALCULATOR_MANAGER_H
#define CALCULATOR_MANAGER_H

#include <atomic>
#include <vector>
#include <memory>
#include <vector>
#include "thread.h"

namespace calculator {

class BaseManager {
public:
    BaseManager(MixersFactory&);
    virtual ~BaseManager();
    BaseManager(const BaseManager&) = delete;

    void SetRandomSeeds(const std::vector<std::uint_fast32_t>& seeds);
    std::vector<ExecutionParameters> GetExecutionParameters() const;

    void Launch(std::size_t threads, DeckSelectors&&, medici::PPatienceSelector&&);

    bool IsRunning() const;
    void Interrupt();
    void IncreaseThreads();
    void DecreaseThreads();

protected:
    std::vector<std::unique_ptr<BaseThread>> threads;
    MixersFactory& mixersFactory;
    std::vector<std::uint_fast32_t> seeds;
    DeckSelectors deckSelector;
    medici::PPatienceSelector patienceSelector;

    virtual void CreateThread(std::size_t number) = 0;
    std::uint_fast32_t GetSeed(std::size_t number) const;
};

class Manager : public BaseManager {
public:
    typedef typename Thread::FoundVector FoundVector;
    Manager(MixersFactory& mixersFactory) : BaseManager(mixersFactory) {}

    FoundVector GetNewDecks() {
        FoundVector result;
        for (auto& thread : threads) {
            auto tmp = static_cast<Thread*>(thread.get())->GetNewDecks();
            result.insert(result.end(), std::make_move_iterator(tmp.begin()),
                          std::make_move_iterator(tmp.end()));
        }
        return result;
    }

private:
    void CreateThread(std::size_t number) override {
        auto mixer = mixersFactory.CreateMixer<Card>(GetSeed(number));
        if (!patienceSelector)
            throw std::logic_error("Passed a nullptr patienceSelector!");
        auto thread = new Thread(deckSelector, patienceSelector, std::move(mixer));
        threads.emplace_back(thread);
        thread->Launch();
    }
};
};

#endif /* CALCULATOR_MANAGER_H */
