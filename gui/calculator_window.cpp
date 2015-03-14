#include "calculator_window.h"

#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include <random>

CalculatorWindow::CalculatorWindow(QWidget* parent) : QDialog(parent), operationInProgress(false), threadsCount(4) {
    QSettings settings;
    restoreGeometry(settings.value("calculator-window:geometry").toByteArray());

    CreateObjects();
    CreateLayout();

    GenerateSeeds();
    DisableButtons(true);
}

void CalculatorWindow::CreateObjects() {
    interruptButton = new QPushButton(tr("Interrupt"));
    QObject::connect(interruptButton, &QPushButton::clicked, this, &CalculatorWindow::InterruptCalculation);

    addThread = new QPushButton(tr("Add a thread"));
    QObject::connect(addThread, &QPushButton::clicked, [this]{
            std::thread(&CalculatorWindow::AddThread, this).detach();
        });
    removeThread = new QPushButton(tr("Remove a thread"));
    QObject::connect(removeThread, &QPushButton::clicked, [this]{
            std::thread(&CalculatorWindow::RemoveThread, this).detach();
        });

    updateProgressTimer = new QTimer();
    updateProgressTimer->setInterval(1000);

    progressBoxes = new QVBoxLayout();
    foundDecks = new QTextEdit(tr("Found decks:") + "\n");
}

void CalculatorWindow::GenerateSeeds() {
    std::random_device rd;
    std::vector<std::uint_fast32_t> randomSeeds;
    for (std::size_t i = 0; i < threadsCount * 2; ++i) {
        randomSeeds.push_back(rd());
    }
    calculatorManager.SetRandomSeeds(randomSeeds);
}

void CalculatorWindow::CreateLayout() {
    auto layout = new QVBoxLayout();
    {
        auto subLayout = new QHBoxLayout();
        subLayout->addWidget(interruptButton);
        subLayout->addWidget(addThread);
        subLayout->addWidget(removeThread);
        layout->addLayout(subLayout);
    }
    {
        auto subLayout = new QHBoxLayout();
        subLayout->addLayout(progressBoxes);
        subLayout->addWidget(foundDecks);
        layout->addLayout(subLayout);
    }
    setLayout(layout);
}

CalculatorWindow::~CalculatorWindow() {
    while (operationInProgress)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CalculatorWindow::closeEvent(QCloseEvent*) {
    QSettings settings;
    settings.setValue("calculator-window:geometry", saveGeometry());
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
            DisableButtons(false);
            std::thread([this](DeckSelectors&& selectors){
                    calculator::Manager::StandardMixer mixer;
                    calculatorManager.Interrupt();
                    calculatorManager.Launch(threadsCount, std::move(selectors), mixer);
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
        DisableButtons(true);
        updateProgressTimer->stop();
        QObject::disconnect(updateProgressTimer);
        std::thread([this]{
                calculatorManager.Interrupt();
                operationInProgress.store(false);
            }).detach();
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
            PopulateParameters(allParameters);
            auto decks = calculatorManager.GetNewDecks();
            PopulateDecks(decks);
            operationInProgress.store(false);
        } else
            operationInProgress.store(false);
    }
}

void CalculatorWindow::PopulateParameters(const std::vector<calculator::Thread::RunParameters>& allParameters){
    if (allParameters.size() > progressVector.size()) {
        for (std::size_t i = progressVector.size(); i != allParameters.size(); ++i) {
            auto progress = new ProgressWidget(i + 1);
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

void CalculatorWindow::PopulateDecks(const calculator::Thread::FoundVector& newDecks){
    if (newDecks.size() > 10)
        foundDecks->append("Too many decks found");
    else {
        for (auto& deck : newDecks)
            AddDeck(deck.first, deck.second);
    }
}

void CalculatorWindow::AddDeck(const calculator::Thread::StandardDeck& deck, const medici::Patience::PatienceInfo& ) {
    QString line("Deck: ");
    for (auto &card : deck) {
        line += cardsTranslations.CardShortName(card) + " ";
    }
    foundDecks->append(line);
}

