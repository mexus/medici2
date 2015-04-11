#include "manager.h"

namespace calculator {

    void Manager::SetRandomSeeds(const std::vector<std::uint_fast32_t>& seeds)
    {
        this->seeds = seeds;
    }

    void Manager::CreateThread(std::size_t number)
    {
        auto thread = new Thread(deckSelector, patienceSelector, GetSeed(number));
        threads.emplace_back(thread);
        thread->Launch();
    }

    void Manager::Launch(std::size_t threadsCount, DeckSelectors&& deckSelector, medici::PPatienceSelector&& patienceSelector)
    {
        if (threads.empty()) {
            this->deckSelector = std::move(deckSelector);
            if (!patienceSelector)
                throw std::logic_error("null patienceSelector supplied");
            this->patienceSelector = std::move(patienceSelector);
            this->mixer = mixer;
            for (std::size_t i = 0; i!= threadsCount; ++i)
                CreateThread(i);
        }
    }

    bool Manager::Running() const
    {
        return !threads.empty();
    }

    void Manager::Interrupt()
    {
        if (!threads.empty())
            threads.clear();
    }

    void Manager::IncreaseThreads()
    {
        if (!threads.empty())
            CreateThread(threads.size());
    }

    void Manager::DecreaseThreads()
    {
        if (threads.size() > 1)
            threads.pop_back();
    }

    Thread::FoundVector Manager::GetNewDecks()
    {
        Thread::FoundVector result;
        for (auto& thread : threads) {
            auto tmp = thread->GetNewDecks();
            result.insert(result.end(),
                    std::make_move_iterator(tmp.begin()),
                    std::make_move_iterator(tmp.end()));
        }
        return result;
    }

    std::vector<Thread::RunParameters> Manager::GetRunParameters() const
    {
        std::vector<Thread::RunParameters> result;
        for (auto& thread : threads) 
            result.push_back(thread->GetRunParameters());
        return result;
    }

    std::uint_fast32_t Manager::GetSeed(std::size_t number) const
    {
        std::size_t count = seeds.size();
        if (count == 0)
            return number;
        else
            return seeds[number % count];
    }

}

