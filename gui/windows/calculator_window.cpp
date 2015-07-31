#include "calculator_window.h"

#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <random>

CalculatorWindow::CalculatorWindow(const CardsTranslations& cardsTranslations,
                                   QWidget* parent)
        : QDialog(parent),
          mixersFactory(N),
          operationInProgress(false),
          calculatorManager(mixersFactory),
          cardsTranslations(cardsTranslations),
          threadsCount(4) {
    QSettings settings;
    restoreGeometry(settings.value("calculator-window:geometry").toByteArray());

    CreateObjects();
    CreateLayout();

    GenerateSeeds();
    DisableButtons(true);

    setModal(true);
}

void CalculatorWindow::CreateObjects() {
    interruptButton = new QPushButton(tr("Interrupt"));
    QObject::connect(interruptButton, &QPushButton::clicked, this,
                     &CalculatorWindow::InterruptCalculation);

    addThread = new QPushButton(tr("Add a thread"));
    QObject::connect(addThread, &QPushButton::clicked, [this] {
        std::thread(&CalculatorWindow::AddThread, this).detach();
    });
    removeThread = new QPushButton(tr("Remove a thread"));
    QObject::connect(removeThread, &QPushButton::clicked, [this] {
        std::thread(&CalculatorWindow::RemoveThread, this).detach();
    });

    updateProgressTimer = new QTimer(this);
    updateProgressTimer->setInterval(1000);

    progressBoxes = new QVBoxLayout();
    foundDecks = new QListWidget();
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
        layout->addLayout(subLayout);
    }
    {
        auto subLayout = new QHBoxLayout();
        {
            auto subSubLayout = new QVBoxLayout();
            subSubLayout->addLayout(progressBoxes);
            subSubLayout->addWidget(addThread);
            subSubLayout->addWidget(removeThread);
            subLayout->addLayout(subSubLayout);
            subLayout->setSizeConstraint(QLayout::SetFixedSize);
        }
        {
            auto subSubLayout = new QVBoxLayout();
            subSubLayout->addWidget(new QLabel(tr("Found decks")));
            subSubLayout->addWidget(foundDecks);
            subLayout->addLayout(subSubLayout);
        }
        layout->addLayout(subLayout);
    }
    setLayout(layout);
}

CalculatorWindow::~CalculatorWindow() {
    while (operationInProgress)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CalculatorWindow::closeEvent(QCloseEvent* e) {
    if (calculatorManager.IsRunning()) {
        if (QMessageBox::question(this, tr("Calculation"),
                                  tr("Interrupt current calculation?")) ==
            QMessageBox::No) {
            e->ignore();
            return;
        } else
            InterruptCalculation();
    }
    foundDecks->clear();
    QSettings settings;
    settings.setValue("calculator-window:geometry", saveGeometry());
}

void CalculatorWindow::Calculate(DeckSelectors&& deckSelectors,
                                 medici::PPatienceSelector&& patienceSelector) {
    if (isHidden())
        show();
    bool isInProgress(false);
    if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
        bool launch = true;
        if (calculatorManager.IsRunning()) {
            auto reply = QMessageBox::question(this, tr("Calculation"),
                                               tr("Interrupt current calculation?"));
            launch = reply == QMessageBox::Yes;
        }
        if (launch) {
            std::thread(
                [this](DeckSelectors&& deckSelectors,
                       medici::PPatienceSelector&& patienceSelector) {
                    calculatorManager.Interrupt();
                    calculatorManager.Launch(threadsCount, std::move(deckSelectors),
                                             std::move(patienceSelector));
                    operationInProgress.store(false);
                },
                std::move(deckSelectors), std::move(patienceSelector)).detach();
            QObject::connect(updateProgressTimer, &QTimer::timeout, this,
                             &CalculatorWindow::ShowProgress);
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
        std::thread([this] {
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
    interruptButton->setVisible(!disabled);
    addThread->setVisible(!disabled);
    removeThread->setVisible(!disabled);
}

void CalculatorWindow::ShowProgress() {
    bool isInProgress(false);
    if (operationInProgress.compare_exchange_strong(isInProgress, true)) {
        if (calculatorManager.IsRunning()) {
            auto allParameters = calculatorManager.GetExecutionParameters();
            PopulateParameters(allParameters);
            auto decks = calculatorManager.GetNewDecks();
            PopulateDecks(decks);
            operationInProgress.store(false);
        } else
            operationInProgress.store(false);
    }
}

void CalculatorWindow::PopulateParameters(
    const std::vector<calculator::ExecutionParameters>& allParameters) {
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

    if (!allParameters.empty())
        DisableButtons(false);

    for (std::size_t i = 0; i != allParameters.size(); ++i)
        progressVector[i]->Set(allParameters[i]);
}

void CalculatorWindow::PopulateDecks(const calculator::Manager::FoundVector& newDecks) {
    static const std::size_t maxDecksAtOnce = 10;
    for (std::size_t i = 0, l = std::min(maxDecksAtOnce, newDecks.size()); i != l; ++i) {
        auto& deck = newDecks[i];
        AddDeck(deck.first, deck.second);
    }
}

void CalculatorWindow::AddDeck(const std::vector<Card>& deck,
                               const medici::PatienceInfo&) {
    QString line;
    for (auto& card : deck) {
        line += cardsTranslations.CardShortName(card) + " ";
    }
    foundDecks->addItem(line);
}
