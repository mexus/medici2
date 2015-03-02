#ifndef GUI_CALCULATION_CONTROLLER_H
#define GUI_CALCULATION_CONTROLLER_H

#include <QObject>

#include <map>

#include "calculation_thread.h"

class CalculationController : public QObject {
	Q_OBJECT
public:
	CalculationController();
	void Start(std::size_t threads);
	void Interrupt();
	std::vector<CalculationThread::StandardDeck> GetDecks();
private:
	std::map<std::size_t, CalculationThread> calculationsThreads;

signals:
	void finished();
};

#endif /* GUI_CALCULATION_CONTROLLER_H */
