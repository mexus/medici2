#include "performance.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>

logxx::Log Performance::cLog("Performance");
using namespace medici;

void Performance::Run(){
	Mixing();
	MediciGenerator();
	MediciWithConditions();
}

void Performance::Mixing(){
	S_LOG("Mixing");
	StandardMixer mixer;
	auto deck = standard_36_deck::Deck::cards;
	// cppcheck-suppress variableScope
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

void Performance::MediciWithConditions(){
	S_LOG("MediciWithConditions");
	using namespace standard_36_deck;
	DeckOneSelector targetCard({CardSelector(Card::Suit(Hearts), Card::Rank(Ten), true)}, 19, 24);
	DeckAllSelector ownActions({CardSelector(Card::Rank(Ace), false)}, 3, 7);
	DeckOneSelector firstCard({CardSelector(Card::Rank(Jack), true)}, 0, 0);
	DeckOneSelector secondCard({CardSelector(Card::Rank(Nine), true)}, 1, 1);
	DeckOneSelector thirdCard({CardSelector(Card::Rank(Ace), true), CardSelector(Card::Rank(Ten), true)}, 2, 2);

	DeckSelectors selectors;
	selectors.AddDeckSelector(targetCard);
	selectors.AddDeckSelector(ownActions);
	selectors.AddDeckSelector(firstCard);
	selectors.AddDeckSelector(secondCard);
	selectors.AddDeckSelector(thirdCard);

	struct CheckOperand{
		CheckOperand(const DeckSelectors& deckSelectors) : deckSelectors(deckSelectors) {}
		bool operator()(const StandardDeck& deck) const {
			return deckSelectors.Check(deck);
		}
	private:
		const DeckSelectors& deckSelectors;
	};

	StandardMixer mixer;
	auto deck = standard_36_deck::Deck::cards;
	Patience::PatienceInfo info;
	CheckOperand checker(selectors);

	static const std::size_t totalDecks = 20;
	{
		TimeMeasure timer;
		for (std::size_t i = 0; i != totalDecks; ++i){
			Generator::Generate(deck, info, mixer, checker);
		}
		double elapsed = timer.Elapsed();
		log(logxx::info) << totalDecks << " appropriate decks generated in " << elapsed << "s: " << totalDecks / elapsed << " decks per second" << logxx::endl;
	}
}

