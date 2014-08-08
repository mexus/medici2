#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <medici/generator.h>
#include <logxx/logxx.h>
#include <cards/standard-36-deck.h>

class Performance {
public:
	void Run();
private:
	typedef standard_36_deck::Deck::ArrayType StandardDeck;
	typedef Mixer<Card, standard_36_deck::Deck::N()> StandardMixer;
	static const std::size_t decksCount = 1E3;

	static logxx::Log cLog;

	static void Mixing();
	static std::vector<StandardDeck> PregenerateConvergableDecks();
	static void MediciGenerator();
};

#endif /* PERFORMANCE_H */

