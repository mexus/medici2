#include "main_form.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <type_traits>
#include <cards/deck-selector.h>

logxx::Log MainForm::cLog("MainForm");

MainForm::MainForm(QWidget* parent) : QMainWindow(parent) {
    QSettings settings;
    restoreGeometry(settings.value("main-window:geometry").toByteArray());
    restoreState(settings.value("main-window:state").toByteArray());

    auto jsonConfig = QJsonDocument::fromBinaryData(
                          settings.value("selector-tabs").toByteArray()).object();
    CreateObjects(jsonConfig);
    CreateLayout();
    tabs->setCurrentIndex(settings.value("main-window:last-selected-tab").toInt());
}

void MainForm::CreateObjects(const QJsonObject& config) {
    setCentralWidget(new QWidget());

    deckPreferenceTab =
        new DeckPreference(cardsTranslations, config["preference-tab"].toObject());

    tabs = new QTabWidget();
    tabs->addTab(deckPreferenceTab, tr("Deck preferences"));

    actionButton = new QPushButton(tr("Calculate"));
    QObject::connect(actionButton, &QPushButton::clicked, this,
                     &MainForm::ActivateCalculation);

    calculator = new CalculatorWindow(cardsTranslations, this);
}

void MainForm::CreateLayout() {
    auto layout = new QVBoxLayout();

    layout->addWidget(tabs);
    layout->addWidget(actionButton);

    centralWidget()->setLayout(layout);
}

MainForm::~MainForm() {
}

void MainForm::closeEvent(QCloseEvent*) {
    QSettings settings;
    settings.setValue("main-window:geometry", saveGeometry());
    settings.setValue("main-window:state", saveState());

    QJsonObject config;
    config["preference-tab"] = deckPreferenceTab->GetConfig();
    settings.setValue("main-window:last-selected-tab", tabs->currentIndex());
    settings.setValue("selector-tabs", QJsonDocument(config).toBinaryData());
}

void MainForm::ActivateCalculation() {
    try {
        auto selectors = deckPreferenceTab->GetDeckSelectors();
        if (!selectors.IsEmpty())
            calculator->Calculate(std::move(selectors),
                                  deckPreferenceTab->GetPatienceSelector());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Unhandled exception"),
                              tr("Please report to developer: ") + QString(e.what()));
    }
}
