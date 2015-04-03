#include "gui_deck_selector.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonArray>
#include <QInputDialog>

GuiDeckSelector::GuiDeckSelector(const CardsTranslations& cardsTranslations, bool newCard) :
    ClicableGroupBox(), cardsTranslations(cardsTranslations)
{
    setTitle(tr("Double click to rename"));
    CreateObjects();
    SetSpinBoxes();
    SetupConnections();
    CreateLayout();
    if (newCard)
        AddCardSelector();
}

GuiDeckSelector::GuiDeckSelector(const CardsTranslations& cardsTranslations, GuiCardSelector* cardSelector, const QString& label) :
    GuiDeckSelector(cardsTranslations, false)
{
    AddCardSelector(cardSelector, false);
    selectorMode->hide();
    enabled->hide();
    addCardButton->hide();
    QObject::disconnect(renameConnection);
    setTitle(label);
}

GuiDeckSelector::GuiDeckSelector(const CardsTranslations& cardsTranslations, const QJsonObject& config) :
    GuiDeckSelector(cardsTranslations, false)
{
    setTitle(config["label"].toString());
    auto index = selectorMode->findData(config["selector_mode"].toInt());
    if (index != -1)
        selectorMode->setCurrentIndex(index);

    auto positionConfig = config["position"].toObject();
    positionStart->setValue(static_cast<int>(positionConfig["start"].toInt(1)));
    positionEnd->setValue(static_cast<int>(positionConfig["end"].toInt(1)));

    enabled->setChecked(config["enabled"].toBool(true));

    auto cards = config["cards"].toArray();
    for (int i = 0; i != cards.size(); ++i) {
        auto cardConfig = cards[i].toObject();
        AddCardSelector(cardConfig);
    }
}

QJsonObject GuiDeckSelector::GetConfig() const {
    QJsonObject config;
    QJsonArray cards;
    for (auto &guiCardSelector: selectors) {
        cards.append(guiCardSelector->GetConfig());
    }
    QJsonObject position;
    position["start"] = positionStart->value();
    position["end"] = positionEnd->value();

    config["label"] = title();
    config["cards"] = cards;
    config["selector_mode"] = selectorMode->currentData().toInt();
    config["position"] = position;
    config["enabled"] = enabled->isChecked();
    return config;
}

void GuiDeckSelector::AddCardSelector() {
    AddCardSelector(new GuiCardSelector(cardsTranslations));
}

void GuiDeckSelector::AddCardSelector(const QJsonObject& config) {
    AddCardSelector(new GuiCardSelector(cardsTranslations, config));
}

void GuiDeckSelector::AddCardSelector(GuiCardSelector* selector, bool removeButton) {
    selectors.insert(selector);
    if (removeButton) {
        auto deleteCard = new QPushButton(tr("Remove the card"));
        selector->AddWidget(deleteCard);
        QObject::connect(deleteCard, &QPushButton::clicked, [this, selector](){
                if (selectors.size()==1)
                    return ;
                selectors.erase(selector);
                selector->disconnect();
                selectorsLayout->removeWidget(selector);
                selector->deleteLater();
                });
    }
    selectorsLayout->addWidget(selector);
}

QSpinBox* GuiDeckSelector::CreateSpinBox(int min, int max) {
    auto box = new QSpinBox();
    box->setRange(min, max);
    box->setSingleStep(1);
    return box;
}

void GuiDeckSelector::SetSpinBoxes() {
    positionStart = CreateSpinBox(1, 36);
    positionEnd = CreateSpinBox(1, 36);
}

void GuiDeckSelector::CreateObjects() {
    selectorMode = new QComboBox();
    selectorMode->addItem(tr("At least one of conditions must match"), SELECT_ONE);
    selectorMode->addItem(tr("All conditions must match"), SELECT_ALL);
    selectorMode->setCurrentIndex(0);

    enabled = new QCheckBox(tr("Enabled"));
    addCardButton = new QPushButton(tr("Add a card"));
}

template<typename... Args> struct Select {
    template<typename C, typename R>
    static constexpr auto Overload( R (C::*pmf)(Args...) ) -> decltype(pmf) {
        return pmf;
    }
};

void GuiDeckSelector::SetupConnections() {
    QObject::connect(addCardButton, &QPushButton::clicked, [this](){AddCardSelector();});
    QObject::connect(positionStart, Select<int>::Overload(&QSpinBox::valueChanged), this, &GuiDeckSelector::ValidatePositionStart);
    QObject::connect(positionEnd, Select<int>::Overload(&QSpinBox::valueChanged), this, &GuiDeckSelector::ValidatePositionEnd);
    renameConnection = QObject::connect(this, &ClicableGroupBox::doubleClicked, this, &GuiDeckSelector::RenameDeck);
}

void GuiDeckSelector::CreateLayout() {
    auto mainLayout = new QHBoxLayout();

    configLayout = new QVBoxLayout();
    configLayout->addWidget(enabled);
    configLayout->addWidget(selectorMode);
    {
        auto layout = new QVBoxLayout();
        {
            auto subLayout = new QHBoxLayout();
            subLayout->addWidget(new QLabel(tr("Position from")));
            subLayout->addWidget(positionStart);
            {
                auto toLabel = new QLabel(tr("to"));
                toLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                subLayout->addWidget(toLabel);
            }
            subLayout->addWidget(positionEnd);
            layout->addLayout(subLayout);
        }
        configLayout->addLayout(layout);

        configLayout->addWidget(addCardButton);
    }
    configLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(configLayout);

    {
        selectorsLayout = new QHBoxLayout();
        {
        }
        selectorsLayout->setSizeConstraint(QLayout::SetFixedSize);
        mainLayout->addLayout(selectorsLayout);
    }
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(mainLayout);
    setFlat(false);
}

void GuiDeckSelector::AddButton(QPushButton* button) {
    configLayout->addWidget(button);
}

std::unique_ptr<DeckAbstractSelector> GuiDeckSelector::GetSelector() const {
    std::unique_ptr<DeckAbstractSelector> selector;
    SelectorMode mode = static_cast<SelectorMode>(selectorMode->currentData().toUInt());

    std::vector<CardSelector> cardSelectors;
    for (auto &guiCardSelector: selectors) {
        bool ok;
        auto cardSelector = guiCardSelector->GetSelector(ok);
        if (ok)
            cardSelectors.push_back(std::move(cardSelector));
        else
            return nullptr;
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

void GuiDeckSelector::ValidatePositionStart(int newValue) {
    if (newValue > positionEnd->value())
        positionEnd->setValue(newValue);
}

void GuiDeckSelector::ValidatePositionEnd(int newValue) {
    int start = positionStart->value();
    if (newValue < start)
        positionEnd->setValue(start);
}

void GuiDeckSelector::RenameDeck() {
    bool ok;
    auto label = QInputDialog::getText(this, tr("Conditions set"), tr("Enter name for a conditions set"), QLineEdit::Normal, title(), &ok);
    if (ok && !label.isEmpty())
        setTitle(label);
}

