#include "deck_preference.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QGroupBox>

DeckPreference::DeckPreference(const CardsTranslations& cardsTranslations) :
    QWidget(), cardsTranslations(cardsTranslations)
{
    targetCard = new GuiCardSelector(cardsTranslations, false);
    CreateLayout();
}

DeckPreference::DeckPreference(const CardsTranslations& cardsTranslations, const QJsonObject& config) :
    QWidget(), cardsTranslations(cardsTranslations)
{
    targetCard = new GuiCardSelector(cardsTranslations, config["target-card"].toObject(), false);

    CreateLayout();
    iChingCheck->setChecked(config["iching-check"].toBool());
    findMaximumConvolutions->setChecked(config["find-maximum"].toBool());
    auto selectors = config["deck-selectors"].toArray();
    for (auto it = selectors.begin(); it != selectors.end(); ++it) {
        AddDeckSelector(new GuiDeckSelector(cardsTranslations, it->toObject()));
    }
}

void DeckPreference::AddDeckSelector(GuiDeckSelector* selector) {
    deckSelectors.insert(selector);
    decksLayout->addWidget(selector);

    auto removeButton = new QPushButton(tr("Remove the conditions set"));
    QObject::connect(removeButton, &QPushButton::clicked, [=](){
            if (QMessageBox::question(this, tr("Deck conditions removal"),
                tr("Do you really want to remove this set of conditions?")) == QMessageBox::Yes)
            {
                RemoveDeckSelector(selector);
            }
        });
    selector->AddButton(removeButton);
}

void DeckPreference::AddNewDeckSelector() {
    AddDeckSelector(new GuiDeckSelector(cardsTranslations));
}

void DeckPreference::RemoveDeckSelector(GuiDeckSelector* selector) {
    deckSelectors.erase(selector);
    decksLayout->removeWidget(selector);
    delete selector;
}

QJsonObject DeckPreference::GetConfig() const {
    QJsonObject config;
    config["target-card"] = targetCard->GetConfig();
    config["iching-check"] = iChingCheck->isChecked();
    config["find-maximum"] = findMaximumConvolutions->isChecked();
    QJsonArray selectors;
    for (auto &selector: deckSelectors)
        selectors.push_back(selector->GetConfig());
    config["deck-selectors"] = selectors;
    return config;
}

void DeckPreference::CreateLayout() {
    auto layout = new QVBoxLayout();
    
    iChingCheck = new QCheckBox(tr("Deck should be I-Ching balanced"));
    layout->addWidget(iChingCheck);


    findMaximumConvolutions = new QCheckBox(tr("Find maximum convolutions"));
    layout->addWidget(findMaximumConvolutions);

    {
        auto addDeck = new QPushButton(tr("Add new set of conditions"));
        addDeck->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QObject::connect(addDeck, &QPushButton::clicked, this, &DeckPreference::AddNewDeckSelector);
        layout->addWidget(addDeck);
    }

    decksLayout = new QVBoxLayout();
    decksLayout->addWidget(new GuiDeckSelector(cardsTranslations, targetCard));
    layout->addLayout(decksLayout);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

medici::PPatienceSelector DeckPreference::GetPatienceSelector() const {
    using namespace medici;
    PPatienceSelector selector;
    auto card = targetCard->GetCard();
    if (findMaximumConvolutions->isChecked())
        selector.reset(new PatienceMaxSelector(card, true));
    else
        selector.reset(new PatienceTargetSelector(card));
    if (iChingCheck->isChecked())
        return PPatienceSelector(new PatienceIChingSelector(std::move(selector)));
    else
        return selector;
}

DeckSelectors DeckPreference::GetDeckSelectors() const {
    DeckSelectors selectors;
    for (auto &guiSelector: deckSelectors) {
        auto selector = guiSelector->GetSelector();
        if (selector)
            selectors.AddDeckSelector(std::move(selector));
    }
    return std::move(selectors);
}


