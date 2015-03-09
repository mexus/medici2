#ifndef GUI_CALCULATION_H
#define GUI_CALCULATION_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

#include <calculator/manager.h>

#include <atomic>

#include "progress.h"

class CalculatorWindow : public QDialog {
	Q_OBJECT
public:
	CalculatorWindow(QWidget* parent = 0);
	~CalculatorWindow();
	void Calculate(DeckSelectors&&);

private:
	std::atomic_bool operationInProgress;
	calculator::Manager calculatorManager;

	std::uint8_t threadsCount;
	QHBoxLayout *layout;
	QPushButton *interruptButton, *addThread, *removeThread;
	QVBoxLayout *progressBoxes;
	std::vector<Progress*> progressVector;

	void InterruptCalculation();
	void AddThread();
	void RemoveThread();

	void DisableButtons(bool disabled);

	template<class T, class ...Args>
	void DefferedCommand(T t, Args&&... args);

	QTimer *updateProgressTimer;
	void ShowProgress();
	void PopulateParameters(std::vector<calculator::Thread::RunParameters>&&);

};

#endif /* GUI_CALCULATION_H */
