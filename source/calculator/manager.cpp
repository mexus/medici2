#include "manager.h"

namespace calculator {

	void Manager::CreateThread() {
		auto thread = new Thread(selector, mixer);
		threads.emplace_back(thread);
		thread->Launch();
	}

	void Manager::Launch(std::size_t threadsCount, DeckSelectors&& selector, const StandardMixer& mixer) {
		if (threads.empty()){
			this->selector = std::move(selector);
			this->mixer = mixer;
			for (std::size_t i = 0; i!= threadsCount; ++i)
				CreateThread();
		}
	}

	void Manager::UpdateParameters(DeckSelectors&& selector) {
		if (threads.empty())
			this->selector = std::move(selector);
	}

	bool Manager::Running() const {
		return !threads.empty();
	}

	void Manager::Interrupt() {
		if (!threads.empty())
			threads.clear();
	}

	void Manager::IncreaseThreads() {
		if (!threads.empty())
			CreateThread();
	}

	void Manager::DecreaseThreads() {
		if (threads.size() > 1)
			threads.pop_back();
	}

	Thread::FoundVector Manager::GetNewDecks() {
		Thread::FoundVector result;
		for (auto& thread : threads) {
			auto tmp = thread->GetNewDecks();
			result.insert(result.end(),
					std::make_move_iterator(tmp.begin()),
					std::make_move_iterator(tmp.end()));
		}
		return result;
	}

	std::vector<Thread::RunParameters> Manager::GetRunParameters() const {
		std::vector<Thread::RunParameters> result;
		for (auto& thread : threads) 
			result.push_back(thread->GetRunParameters());
		return result;
	}

}

