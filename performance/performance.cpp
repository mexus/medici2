#include "performance.h"
#include <helpers/time_measure.h>

logxx::Log Performance::cLog("Performance");
using namespace medici;

void Performance::Run(){
	Mixer();
	MediciGenerator();
}

void Performance::Mixer(){
	S_LOG("Mixer");
	StandardMixer mixer;
	auto deck = standard_36_deck::Deck::cards;
	std::size_t decksCount = 1E6;
	{
		TimeMeasure timer;
		for (std::size_t i = 0; i != decksCount; ++i){
			mixer.Mix(deck);
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << decksCount << " decks generated in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
	}
}

std::vector<Performance::StandardDeck> Performance::PregenerateConvergableDecks(){
	S_LOG("PregenerateConvergableDecks");
	StandardMixer mixer;
	Patience::PatienceInfo info;
	std::vector<StandardDeck> pregeneratedDecks;
	pregeneratedDecks.reserve(decksCount);
	auto deck = standard_36_deck::Deck::cards;
	{
		TimeMeasure timer;
		for (std::size_t i = 0; i != decksCount; ++i){
			Generator::Generate(deck, info, mixer);
			pregeneratedDecks.push_back(deck);
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << decksCount << " convergable decks generated in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
	}
	return pregeneratedDecks;
}

void Performance::MediciGenerator(){
	S_LOG("MediciGenerator");
	StandardMixer mixer;
	Patience::PatienceInfo info;

	auto pregeneratedDecks = PregenerateConvergableDecks();
	auto it = pregeneratedDecks.cbegin();
	auto end = pregeneratedDecks.cend();
	{
		TimeMeasure timer;
		for (; it != end; ++it){
			auto &deck = *it;
			if (!Patience::Converge(deck, info))
				throw std::logic_error("Convergable deck doesn't converges!");
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << decksCount << " convergable decks converged in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
	}

}

