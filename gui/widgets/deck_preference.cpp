#include "deck_preference.h"

#include <QVBoxLayout>

DeckPreference::DeckPreference(QWidget* parent) : QWidget(parent)
{
    targetCard = new GuiCardSelector(false, false);
    CreateLayout();
}

DeckPreference::DeckPreference(const QJsonObject& config, QWidget *parent) : QWidget(parent) {
    targetCard = new GuiCardSelector(config["target-card"].toObject(), false, false);

    CreateLayout();

    iChingCheck->setChecked(config["iching-check"].toBool());
    findMaximumConvolutions->setChecked(config["find-maximum"].toBool());
}

QJsonObject DeckPreference::GetConfig() const {
    QJsonObject config;
    config["target-card"] = targetCard->GetConfig();
    config["iching-check"] = iChingCheck->isChecked();
    config["find-maximum"] = findMaximumConvolutions->isChecked();
    return config;
}

void DeckPreference::CreateLayout() {
    auto layout = new QVBoxLayout();
    
    iChingCheck = new QCheckBox(tr("Deck should be I-Ching balanced"));
    layout->addWidget(iChingCheck);

    targetCard->HideRemoveButton();
    layout->addWidget(targetCard);

    findMaximumConvolutions = new QCheckBox(tr("Find maximum convolutions"));
    layout->addWidget(findMaximumConvolutions);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

medici::PPatienceSelector DeckPreference::GetSelector() const {
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

