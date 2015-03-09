#include "calculation.h"
#include <QLabel>
#include <QMessageBox>

CalculatorWindow::CalculatorWindow(QWidget* parent) : QDialog(parent), operationInProgress(false), threadsCount(4) {
	auto layout = new QHBoxLayout();

	interruptButton = new QPushButton(tr("Interrupt"));
	QObject::connect(interruptButton, &QPushButton::clicked, [this]{
			std::thread(&CalculatorWindow::InterruptCalculation, this).detach();
			updateProgressTimer->stop();
		});
	layout->addWidget(interruptButton);

	addThread = new QPushButton(tr("Add a thread"));
	QObject::connect(addThread, &QPushButton::clicked, [this]{
			std::thread(&CalculatorWindow::AddThread, this).detach();
		});
	layout->addWidget(addThread);

	removeThread = new QPushButton(tr("Remove a thread"));
	QObject::connect(removeThread, &QPushButton::clicked, [this]{
			std::thread(&CalculatorWindow::RemoveThread, this).detach();
		});
	layout->addWidget(removeThread);

	progressBoxes = new QVBoxLayout();
	layout->addLayout(progressBoxes);

	updateProgressTimer = new QTimer();
	updateProgressTimer->setInterval(1000);

	DisableButtons(true);

	setLayout(layout);
}

CalculatorWindow::~CalculatorWindow() {
	while (operationInProgress)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CalculatorWindow::Calculate(DeckSelectors&& selectors) {
	if (isHidden())
		show();
	bool isInProgress(false);
	if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
		bool launch = true;
		if (calculatorManager.Running()){
			auto reply = QMessageBox::question(this, tr("Calculation"), tr("Interrupt current calculation?"));
			launch = reply == QMessageBox::Yes;
		}
		if (launch){
			std::thread([this](DeckSelectors&& selectors){
					calculator::Manager::StandardMixer mixer;
					calculatorManager.Interrupt();
					calculatorManager.Launch(threadsCount, std::move(selectors), mixer);	
					DisableButtons(false);
					operationInProgress.store(false);
				}, std::move(selectors)).detach();
				QObject::connect(updateProgressTimer, &QTimer::timeout, this, &CalculatorWindow::ShowProgress);
				updateProgressTimer->start();
		} else
			operationInProgress.store(false);
	}
}

void CalculatorWindow::InterruptCalculation() {
	bool isInProgress(false);
	if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
		QObject::disconnect(updateProgressTimer);
		calculatorManager.Interrupt();
		DisableButtons(true);
		operationInProgress.store(false);
	}
}

void CalculatorWindow::AddThread() {
	bool isInProgress(false);
	if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
		calculatorManager.IncreaseThreads();
		operationInProgress.store(false);
	}
}

void CalculatorWindow::RemoveThread() {
	bool isInProgress(false);
	if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
		calculatorManager.DecreaseThreads();
		operationInProgress.store(false);
	}
}

void CalculatorWindow::DisableButtons(bool disabled) {
	interruptButton->setDisabled(disabled);
	addThread->setDisabled(disabled);
	removeThread->setDisabled(disabled);
}

void CalculatorWindow::ShowProgress() {
	bool isInProgress(false);
	if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
		if (calculatorManager.Running()) {
			auto allParameters = calculatorManager.GetRunParameters();
			PopulateParameters(std::move(allParameters));
			operationInProgress.store(false);
		} else
			operationInProgress.store(false);
	}
}

#include <QDebug>
void CalculatorWindow::PopulateParameters(std::vector<calculator::Thread::RunParameters>&& allParameters){
	if (allParameters.size() > progressVector.size()) {
		for (std::size_t i = progressVector.size(); i != allParameters.size(); ++i) {
			auto progress = new Progress(i);
			progressVector.push_back(progress);
			progressBoxes->addWidget(progress);
		}
	} else if (allParameters.size() < progressVector.size()) {
		while (progressVector.size() != allParameters.size()) {
			auto last = progressVector.back();
			progressBoxes->removeWidget(last);
			progressVector.pop_back();
			delete last;
		}
	}

	for (std::size_t i = 0; i != allParameters.size(); ++i)
		progressVector[i]->Set(allParameters[i]);
}

