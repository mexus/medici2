#ifndef CALCULATOR_MANAGER_H
#define CALCULATOR_MANAGER_H

#include <atomic>
#include <vector>
#include <memory>
#include <vector>

#include "thread.h"

namespace calculator {

	class Manager {
	public:
		typedef Thread::StandardDeck StandardDeck;
		typedef Thread::StandardMixer StandardMixer;

		Manager() = default;
		Manager(const Manager&) = delete;

		void Launch(std::size_t threads, DeckSelectors&& selector, const StandardMixer& mixer);
		void UpdateParameters(DeckSelectors&&);
		void SetRandomSeeds(const std::vector<std::uint_fast32_t>& seeds);

		bool Running() const;
		void Interrupt();
		void IncreaseThreads();
		void DecreaseThreads();

		Thread::FoundVector GetNewDecks();
		std::vector<Thread::RunParameters> GetRunParameters() const;
	private:
		std::vector<std::uint_fast32_t> seeds;
		DeckSelectors selector;
		StandardMixer mixer;

		std::vector<std::unique_ptr<Thread>> threads;

		void CreateThread(std::size_t number);
		std::uint_fast32_t GetSeed(std::size_t number) const;
	};

};

#endif /* CALCULATOR_MANAGER_H */
