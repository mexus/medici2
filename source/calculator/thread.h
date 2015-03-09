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
#include <medici/generator.h>

namespace calculator {

	class Thread {
	public:
		typedef standard_36_deck::Deck::ArrayType StandardDeck;
		typedef Mixer<Card, standard_36_deck::Deck::N()> StandardMixer;

		typedef std::pair<StandardDeck, medici::Patience::PatienceInfo> FoundType;
		typedef std::vector<FoundType> FoundVector;

		struct RunParameters {
			std::uintmax_t checkedDecks, suitableDecks;
			std::chrono::steady_clock::duration runningTime;
		};

		Thread(const DeckSelectors&, const StandardMixer& mixer);
		~Thread();

		void Launch();
		RunParameters GetRunParameters() const;
		FoundVector GetNewDecks();

	private:
		const DeckSelectors& selector;
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
