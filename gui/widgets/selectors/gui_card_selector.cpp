#include "gui_card_selector.h"

#include <QLayout>
#include <QTimer>
#include <type_traits>
#include <cards/standard-36-deck.h>

template <class T>
typename std::enable_if<std::is_same<T, QComboBox>::value>::type SelectData(T* comboBox,
                                                                            int data) {
    int index = comboBox->findData(data);
    if (index == -1)
        throw std::runtime_error("Unknown data");
    comboBox->setCurrentIndex(index);
}

GuiCardSelector::GuiCardSelector(const CardsTranslations& cardsTranslations,
                                 bool multipleAllowed)
        : QFrame(),
          cardsTranslations(cardsTranslations),
          multipleAllowed(multipleAllowed) {
    CreateObjects();
    PopulateSuits();
    PopulateRanks();
    CreateLayout();
    SelectData(this->suit, multipleAllowed ? -1 : 0);
    SelectData(this->rank, multipleAllowed ? -1 : 0);
}

GuiCardSelector::GuiCardSelector(const CardsTranslations& cardsTranslations,
                                 const QJsonObject& config, bool multipleAllowed)
        : GuiCardSelector(cardsTranslations, multipleAllowed) {
    SelectData(this->suit, config["suit"].toInt());
    SelectData(this->rank, config["rank"].toInt());
    inverse->setChecked(config["inversed"].toBool());
}

QJsonObject GuiCardSelector::GetConfig() const {
    QJsonObject config;
    config["suit"] = suit->currentData().toInt();
    config["rank"] = rank->currentData().toInt();
    config["inversed"] = inverse->isChecked();
    return config;
}

void GuiCardSelector::CreateObjects() {
    suit = new QComboBox();
    rank = new QComboBox();
    inverse = new QCheckBox(tr("Inverse"));
    if (!multipleAllowed)
        inverse->setVisible(false);
}

class Populater {
public:
    Populater(QComboBox* box) : box(box) {}
    void Add(const QString& title, int value) { box->addItem(title, value); }

private:
    QComboBox* box;
};

void GuiCardSelector::PopulateSuits() {
    Populater p(suit);
    if (multipleAllowed)
        p.Add(tr("Any suit"), -1);
    std::array<standard_36_deck::Suits, 4> suits{
        {standard_36_deck::Suits::Diamonds, standard_36_deck::Suits::Spades,
         standard_36_deck::Suits::Hearts, standard_36_deck::Suits::Clubs}};
    for (auto& suit : suits)
        p.Add(cardsTranslations.SuitLongName(suit), suit);
}

void GuiCardSelector::PopulateRanks() {
    Populater p(rank);
    if (multipleAllowed)
        p.Add(tr("Any rank"), -1);
    std::array<standard_36_deck::Ranks, 9> ranks{
        {standard_36_deck::Ranks::Six, standard_36_deck::Ranks::Seven,
         standard_36_deck::Ranks::Eight, standard_36_deck::Ranks::Nine,
         standard_36_deck::Ranks::Ten, standard_36_deck::Ranks::Jack,
         standard_36_deck::Ranks::Queen, standard_36_deck::Ranks::King,
         standard_36_deck::Ranks::Ace}};
    for (auto& rank : ranks)
        p.Add(cardsTranslations.RankLongName(rank), rank);
}

void GuiCardSelector::CreateLayout() {
    innerLayout = new QVBoxLayout();
    innerLayout->addWidget(suit);
    innerLayout->addWidget(rank);
    innerLayout->addWidget(inverse);

    innerLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(innerLayout);

    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
}

void GuiCardSelector::AddWidget(QWidget* widget) {
    innerLayout->addWidget(widget);
}

CardSelector GuiCardSelector::GetSelector(bool& ok) {
    int suitData = suit->currentData().toInt();
    int rankData = rank->currentData().toInt();

    CardSelectorConfigurator configurator;
    configurator.SetStraight(!inverse->isChecked());

    ok = true;
    if (suitData == -1 && rankData == -1) {
        Highlight();
        ok = false;
        configurator.SetRank(0);
        configurator.SetSuit(0);
    } else {
        if (suitData != -1)
            configurator.SetSuit(suitData);
        if (rankData != -1)
            configurator.SetRank(rankData);
    }
    return configurator.GetSelector();
}

Card GuiCardSelector::GetCard() const {
    int suitData = suit->currentData().toInt();
    if (suitData == -1)
        throw std::logic_error("Can't detect a suit");
    int rankData = rank->currentData().toInt();
    if (rankData == -1)
        throw std::logic_error("Can't detect a rank");
    return {std::uint_fast8_t(suitData), std::uint_fast8_t(rankData)};
}

void GuiCardSelector::Highlight() {
    static const int highlightedStyle = QFrame::Panel | QFrame::Sunken;
    int style = frameStyle();
    if (style != highlightedStyle) {
        int width = lineWidth();
        setFrameStyle(highlightedStyle);
        setLineWidth(3);

        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        QTimer::connect(timer, &QTimer::timeout, [=]() {
            setFrameStyle(style);
            setLineWidth(width);
            timer->deleteLater();
        });
        timer->start(5000);
    }
}
