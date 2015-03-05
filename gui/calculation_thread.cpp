#include "calculation_thread.h"

std::vector<CalculationThread::StandardDeck> CalculationThread::GetDecks() {
	QMutexLocker locker(&accessDecks);
	return allDecks;
}

CalculationThread::CalculationThread(const DeckSelectors& selectors) : QThread(),
	deckSelectors(selectors), interrupt(false)
{
}

void CalculationThread::Interrupt() {
	interrupt = true;
}

void CalculationThread::run() {
	auto deck = StandardDeckType::cards;
	while(!interrupt) {
		medici::Patience::PatienceInfo info;
		StandardMixer mixer;
		medici::generator::Generate(deck, info, mixer, interrupt, deckSelectors);
		if (!interrupt || deckSelectors(deck)) {
			QMutexLocker locker(&accessDecks);
			allDecks.push_back(deck);
		}
	}
}

