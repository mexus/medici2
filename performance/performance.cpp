#include "performance.h"
#include <helpers/time_measure.h>

logxx::Log Performance::cLog("Performance");

void Performance::Run(){
	Mixer();
	MediciGenerator();
}

void Performance::Mixer(){
	static logxx::Log log("Mixer");
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
	static logxx::Log log("PregenerateConvergableDecks");
	StandardMixer mixer;
	Medici::PatienceInfo info;
	std::vector<StandardDeck> pregeneratedDecks;
	pregeneratedDecks.reserve(decksCount);
	auto deck = standard_36_deck::Deck::cards;
	{
		TimeMeasure timer;
		for (std::size_t i = 0; i != decksCount; ++i){
			MediciGenerator::Generate(deck, info, mixer);
			pregeneratedDecks.push_back(deck);
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << decksCount << " convergable decks generated in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
	}
	return pregeneratedDecks;
}

void Performance::MediciGenerator(){
	static logxx::Log log("MediciGenerator");
	StandardMixer mixer;
	Medici::PatienceInfo info;

	auto pregeneratedDecks = PregenerateConvergableDecks();
	auto it = pregeneratedDecks.cbegin();
	auto end = pregeneratedDecks.cend();
	{
		TimeMeasure timer;
		for (; it != end; ++it){
			auto &deck = *it;
			if (!Medici::Converge(deck, info))
				throw std::logic_error("Convergable deck doesn't converges!");
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << decksCount << " convergable decks converged in " << elapsed << "s: " << decksCount / elapsed << " decks per second" << logxx::endl;
	}

}

