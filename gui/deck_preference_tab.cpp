#include "deck_preference_tab.h"
#include <QVBoxLayout>

DeckPreference::DeckPreference(QWidget* parent) : QWidget(parent)
{
    targetCard = new GuiCardSelector(false, false);
    CreateLayout();
}

DeckPreference::DeckPreference(const Config& config, QWidget *parent) : QWidget(parent) {
    targetCard = new GuiCardSelector({
            config.targetCard.suit.value,
            config.targetCard.rank.value,
            false
            }, false, false);
    CreateLayout();
}

void DeckPreference::CreateLayout() {
    auto layout = new QVBoxLayout();
    
    iChingCheck = new QCheckBox(tr("Deck should be I-Ching balanced"));
    layout->addWidget(iChingCheck);

    layout->addWidget(targetCard);

    findMaximumConvolutions = new QCheckBox(tr("Find maximum convolutions"));
    layout->addWidget(findMaximumConvolutions);

    setLayout(layout);
}

