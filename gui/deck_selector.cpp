#include "deck_selector.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonArray>

QJsonObject GuiDeckSelector::GetConfig() const {
    QJsonObject config;
    QJsonArray cards;
    for (auto &guiCardSelector: selectors) {
        cards.append(guiCardSelector->GetConfig());
    }
    QJsonObject position;
    position["start"] = positionStart->value();
    position["end"] = positionEnd->value();

    config["cards"] = cards;
    config["selector_mode"] = selectorMode->currentData().toInt();
    config["position"] = position;
    config["enabled"] = enabled->isChecked();
    return config;
}

GuiDeckSelector::GuiDeckSelector(QWidget *parent) : QWidget(parent) {
    CreateElements();
    SetSpinBoxes();
    CreateLayout();
}

GuiDeckSelector::GuiDeckSelector(const QJsonObject& config, QWidget *parent) : GuiDeckSelector(parent) {
    auto index = selectorMode->findData(config["selector_mode"].toInt());
    if (index != -1)
        selectorMode->setCurrentIndex(index);

    auto positionConfig = config["position"].toObject();
    positionStart->setValue(static_cast<int>(positionConfig["begin"].toInt()));
    positionEnd->setValue(static_cast<int>(positionConfig["end"].toInt()));

    enabled->setChecked(config["enabled"].toBool());

    auto cards = config["cards"].toArray();
    for (int i = 0; i != cards.size(); ++i) {
        auto cardConfig = cards[i].toObject();
        AddCardSelector(cardConfig);
    }
}

void GuiDeckSelector::AddCardSelector() {
    AddCardSelector(new GuiCardSelector());
}

void GuiDeckSelector::AddCardSelector(const QJsonObject& config) {
    AddCardSelector(new GuiCardSelector(config));
}

void GuiDeckSelector::AddCardSelector(GuiCardSelector* selector) {
    selectors.insert(selector);
    QObject::connect(selector, &GuiCardSelector::DeleteClicked, [this, selector](){
            if (selectors.size()==1)
                return ;
            selectors.erase(selector);
            selector->disconnect();
            selectorsLayout->removeWidget(selector);
            selector->deleteLater();
            });
    selectorsLayout->insertWidget(selectorsLayout->count() - 1, selector);
}

QSpinBox* GuiDeckSelector::CreateSpinBox(int min, int max) {
    auto box = new QSpinBox();
    box->setRange(min, max);
    box->setSingleStep(1);
    return box;
}

void GuiDeckSelector::CreateElements() {
    selectorMode = new QComboBox();
    selectorMode->addItem(tr("At least one of conditions must match"), SELECT_ONE);
    selectorMode->addItem(tr("All conditions must match"), SELECT_ALL);
    selectorMode->setCurrentIndex(0);

    enabled = new QCheckBox(tr("Is enabled"));
}

void GuiDeckSelector::SetSpinBoxes() {
    positionStart = CreateSpinBox(1, 36);
    positionEnd = CreateSpinBox(1, 36);
}

void GuiDeckSelector::CreateLayout() {
    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(enabled);
    mainLayout->addWidget(selectorMode);
    {
        auto layout = new QHBoxLayout();
        layout->addWidget(new QLabel(tr("Position from: ")));
        layout->addWidget(positionStart);
        layout->addWidget(new QLabel(tr(" to: ")));
        layout->addWidget(positionEnd);
        mainLayout->addLayout(layout);
    }
    {
        selectorsLayout = new QHBoxLayout();
        {
            auto button = new QPushButton(tr("Add a card"));
            selectorsLayout->addWidget(button);
            QObject::connect(button, &QPushButton::clicked, [this](){AddCardSelector();});
        }
        mainLayout->addLayout(selectorsLayout);
    }
    {
        auto deleteButton = new QPushButton(tr("Remove conditions set"));
        QObject::connect(deleteButton, &QPushButton::clicked, this, &GuiDeckSelector::DeleteClicked);
        mainLayout->addWidget(deleteButton);
    }
    setLayout(mainLayout);
}

std::unique_ptr<DeckAbstractSelector> GuiDeckSelector::GetSelector() const {
    if (selectors.empty())
        throw NoCards();

    std::unique_ptr<DeckAbstractSelector> selector;
    SelectorMode mode = static_cast<SelectorMode>(selectorMode->currentData().toUInt());

    std::vector<CardSelector> cardSelectors;
    for (auto &guiCardSelector: selectors) {
        cardSelectors.push_back(std::move(guiCardSelector->GetSelector()));
    }
    std::size_t from = static_cast<std::size_t>(positionStart->value());
    std::size_t to = static_cast<std::size_t>(positionEnd->value());

    switch (mode) {
        case SELECT_ONE:
            selector.reset(new DeckOneSelector(std::move(cardSelectors), from, to));
            break;
        case SELECT_ALL:
            selector.reset(new DeckAllSelector(std::move(cardSelectors), from, to));
            break;
        default:
            throw std::logic_error("Unknown mode");
    }
    return selector;
}

