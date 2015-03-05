#ifndef GUI_CALCULATION_CONTROLLER_H
#define GUI_CALCULATION_CONTROLLER_H

#include <QObject>
#include <QMutex>

#include <set>

#include "calculation_thread.h"

class CalculationController : public QObject {
	Q_OBJECT
public:
	typedef CalculationThread::StandardDeck StandardDeck;

	CalculationController(DeckSelectors&&);
	void Start(std::size_t threads);
	void Interrupt();
	void IncreaseThreads();
	void DecreaseThreads();

	std::vector<StandardDeck> GetDecks();
private:
	const DeckSelectors deckSelectors;
	QMutex threadsMutex;

	std::set<CalculationThread*> calculationsThreads;

	void ThreadDone(CalculationThread*);
	void Interrupt(CalculationThread*);

	void RemoveThread(CalculationThread*);
	void AddThread();
signals:
	void Finished();
	void Started();
	void ThreadsCountChanged(std::size_t newCount);
};

#endif /* GUI_CALCULATION_CONTROLLER_H */
