#include "calculation_thread.h"

std::vector<CalculationThread::StandardDeck> CalculationThread::GetDecks() {
	QMutexLocker locker(&accessDecks);
	return allDecks;
}

CalculationThread::CalculationThread(const BeforeFunctor& beforeFunctor, const AfterFunctor& afterFunctor) : QThread(),
	beforeFunctor(beforeFunctor), afterFunctor(afterFunctor), interrupt(false)
{
}

void CalculationThread::run() {
	StandardDeck deck;
	while(!interrupt) {
		medici::Patience::PatienceInfo info;
		StandardMixer mixer;
		medici::generator::Generate(deck, info, mixer, interrupt, beforeFunctor, afterFunctor);
		if (!interrupt || (beforeFunctor(deck) && afterFunctor(deck, info))) {
			QMutexLocker locker(&accessDecks);
			allDecks.push_back(deck);
		}
	}
}

