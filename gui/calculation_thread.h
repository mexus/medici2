#ifndef GUI_CALCULATION_THREAD_H
#define GUI_CALCULATION_THREAD_H

#include <medici/generator.h>
#include <cards/standard-36-deck.h>

#include <QMutex>
#include <QThread>

class CalculationThread : public QThread {
	Q_OBJECT
public:
	typedef standard_36_deck::Deck StandardDeckType;

	typedef StandardDeckType::ArrayType StandardDeck;
	typedef Mixer<Card, StandardDeckType::N()> StandardMixer;
	typedef medici::generator::BeforeFunctor<StandardDeckType::N()> BeforeFunctor;
	typedef medici::generator::AfterFunctor<StandardDeckType::N()> AfterFunctor;

	CalculationThread(const BeforeFunctor& beforeFunctor, const AfterFunctor& afterFunctor);
	std::vector<StandardDeck> GetDecks();
	void Interrupt();
private:
	const BeforeFunctor& beforeFunctor;
	const AfterFunctor& afterFunctor;

	std::atomic_bool interrupt;

	QMutex accessDecks;
	std::vector<StandardDeck> allDecks;

	void run() override;

};

#endif /* GUI_CALCULATION_THREAD */
