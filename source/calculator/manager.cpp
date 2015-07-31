#include "manager.h"

namespace calculator {

BaseManager::BaseManager(MixersFactory& mixersFactory) : mixersFactory(mixersFactory) {
}

BaseManager::~BaseManager() {
    threads.clear();
}

void BaseManager::Launch(std::size_t threadsCount, DeckSelectors&& deckSelector,
                         medici::PPatienceSelector&& patienceSelector) {
    if (threads.empty()) {
        this->deckSelector = std::move(deckSelector);
        this->patienceSelector = std::move(patienceSelector);
        for (std::size_t i = 0; i != threadsCount; ++i)
            CreateThread(i);
    }
}

void BaseManager::SetRandomSeeds(const std::vector<std::uint_fast32_t>& seeds) {
    this->seeds = seeds;
}

std::uint_fast32_t BaseManager::GetSeed(std::size_t number) const {
    std::size_t count = seeds.size();
    if (count == 0)
        return number;
    else
        return seeds[number % count];
}

bool BaseManager::IsRunning() const {
    return !threads.empty();
}

void BaseManager::Interrupt() {
    threads.clear();
}

void BaseManager::IncreaseThreads() {
    if (!threads.empty())
        CreateThread(threads.size());
}

void BaseManager::DecreaseThreads() {
    if (threads.size() > 1)
        threads.pop_back();
}

std::vector<ExecutionParameters> BaseManager::GetExecutionParameters() const {
    std::vector<ExecutionParameters> result;
    for (auto& thread : threads)
        result.push_back(thread->GetExecutionParameters());
    return result;
}
}
