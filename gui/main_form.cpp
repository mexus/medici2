#include "main_form.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <type_traits>
#include <cards/deck-selector.h>
#include "settings_helper.h"

logxx::Log MainForm::cLog("MainForm");

void MainForm::AddSelectorTab(GuiDeckSelector* selector, const QString& label) {
    tabs->addTab(selector, label);

    QObject::connect(selector, &GuiDeckSelector::DeleteClicked, [this, selector](){
            if (tabs->count() > 2) {
                auto index = tabs->indexOf(selector);
                tabs->removeTab(index);
                selector->deleteLater();
            }
            });
}

void MainForm::RenameSelector(int index) {
    auto selector = dynamic_cast<GuiDeckSelector*>(tabs->widget(index));
    if (selector) {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Conditions name"),
                tr("Condition name:"), QLineEdit::Normal,
                tabs->tabText(index), &ok);
        if (ok && !text.isEmpty())
        tabs->setTabText(index, text);
    }
}

MainForm::MainForm(QWidget* parent) : QMainWindow(parent) {
    QSettings settings;
    restoreGeometry(settings.value("main-window:geometry").toByteArray());
    restoreState(settings.value("main-window:state").toByteArray());

    setCentralWidget(new QWidget());
    auto layout = new QVBoxLayout();

    {
        auto addButton = new QPushButton(tr("Add conditions set"));
        QObject::connect(addButton, &QPushButton::clicked, [this](){AddSelectorTab();});
        layout->addWidget(addButton);
    }

    auto jsonConfig = QJsonDocument::fromBinaryData(settings.value("selector-tabs").toByteArray()).object();

    deckPreferenceTab = new DeckPreference(jsonConfig["preference-tab"].toObject());

    tabs = new QTabWidget();
    tabs->addTab(deckPreferenceTab, tr("Deck preferences"));
    LoadSelectorTabs(jsonConfig["selector-tabs"].toArray());
    QObject::connect(tabs, &QTabWidget::tabBarDoubleClicked, this, &MainForm::RenameSelector);

    layout->addWidget(tabs);

    {
        actionButton = new QPushButton(tr("Calculate"));
        QObject::connect(actionButton, &QPushButton::clicked, this, &MainForm::ActivateCalculation);
        layout->addWidget(actionButton);

    }

    calculator = new CalculatorWindow(this);

    centralWidget()->setLayout(layout);
}

MainForm::~MainForm() {
}

void MainForm::closeEvent(QCloseEvent*) {
    QSettings settings;
    settings.setValue("main-window:geometry", saveGeometry());
    settings.setValue("main-window:state", saveState());

    QJsonObject config;
    config["selector-tabs"] = SaveSelectorTabs();
    config["preference-tab"] = deckPreferenceTab->GetConfig();
    settings.setValue("selector-tabs", QJsonDocument(config).toBinaryData());
}

QJsonArray MainForm::SaveSelectorTabs() const {
    QJsonArray array;
    for (int i = 0; i != tabs->count(); ++i) {
        auto selector = dynamic_cast<GuiDeckSelector*>(tabs->widget(i));
        if (selector) {
            QJsonObject selectorTab;
            selectorTab["label"] = tabs->tabText(i);
            selectorTab["selector"] = selector->GetConfig();
            array.append(selectorTab);
        }
    }
    return array;
}

void MainForm::LoadSelectorTabs(const QJsonArray& array) {
    std::size_t selectorsCount = array.size();
    if (selectorsCount == 0)
        AddSelectorTab();
    for (std::size_t i = 0; i != selectorsCount; ++i) {
        auto selectorTab = array[i].toObject();
        auto selector = selectorTab["selector"].toObject();
        auto label = selectorTab["label"].toString();
        AddSelectorTab(new GuiDeckSelector(selector), label);
    }
}

DeckSelectors MainForm::GetSelectors() {
    DeckSelectors selectors;
    int tabsCount = tabs->count();
    for (int i = 0; i != tabsCount; ++i) {
        auto selectorGui = dynamic_cast<GuiDeckSelector*>(tabs->widget(i));
        if (selectorGui) {
            try {
                auto selector = selectorGui->GetSelector();
                if (selector)
                    selectors.AddDeckSelector(std::move(selector));
            } catch (const GuiCardSelector::NoSuitNoRank &) {
                QMessageBox::critical(this, tabs->tabText(i), tr("You should select at least suit or rank for each card!"));
                tabs->setCurrentIndex(i);
                return DeckSelectors();
            } catch (const GuiDeckSelector::NoCards &) {
                QMessageBox::critical(this, tabs->tabText(i), tr("You have not added any cards to a conditions set!"));
                tabs->setCurrentIndex(i);
                return DeckSelectors();
            }
        }
    }
    return selectors;
}

void MainForm::ActivateCalculation() {
    try {
        auto selectors = GetSelectors();
        if (!selectors.IsEmpty())
            calculator->Calculate(std::move(selectors));
    } catch (const std::exception& e){
        QMessageBox::critical(this, tr("Unhandled exception"), tr("Please report to developer: ") + QString(e.what()));
    }
}

