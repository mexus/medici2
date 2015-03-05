#include "calculation_controller.h"

CalculationController::CalculationController(DeckSelectors&& selectors, QObject* parent) :
	QObject(parent), deckSelectors(std::move(selectors))
{
}

void CalculationController::Start(std::size_t threads) {
	for (std::size_t i = 0; i != threads; ++i) {
		AddThread();
	}
	emit Started();
}

void CalculationController::ThreadDone(CalculationThread* thread) {
	Interrupt(thread);
	std::size_t threadsCount;
	{
		QMutexLocker locker(&threadsMutex);
		threadsCount = calculationsThreads.size();
	}
	emit ThreadsCountChanged(threadsCount);
	if (threadsCount == 0)
		emit Finished();
}

void CalculationController::Interrupt(CalculationThread* thread) {
	thread->Interrupt();
	RemoveThread(thread);
	thread->deleteLater();
}

void CalculationController::RemoveThread(CalculationThread* thread) {
	QMutexLocker locker(&threadsMutex);
	auto it = calculationsThreads.find(thread);
	if (it != calculationsThreads.end())
		calculationsThreads.erase(it);
}

void CalculationController::AddThread() {
	auto thread = new CalculationThread(deckSelectors);
	QObject::connect(thread, &CalculationThread::finished, [this, thread](){ThreadDone(thread);});
	thread->start();
	QMutexLocker locker(&threadsMutex);
	calculationsThreads.insert(thread);
}

void CalculationController::Interrupt() {
	for (auto& thread : calculationsThreads) {
		Interrupt(thread);
	}
}

void CalculationController::DecreaseThreads() {
	if (calculationsThreads.size() > 1) {
		auto& thread = *calculationsThreads.begin();
		Interrupt(thread);
	}
}

void CalculationController::IncreaseThreads() {
	AddThread();
}

