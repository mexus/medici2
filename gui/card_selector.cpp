#include "card_selector.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

#include <type_traits>

#include <cards/standard-36-deck.h>

GuiCardSelector::Config GuiCardSelector::GetConfig() const {
	return {suit->currentData().toInt(),
		rank->currentData().toInt(),
		inverse->isChecked()};
}

template<class T>
typename std::enable_if<std::is_same<T, QComboBox>::value>::type SelectData(T* comboBox, int data) {
	int index = comboBox->findData(data);
	if (index == -1)
		throw std::runtime_error("Unknown data");
	comboBox->setCurrentIndex(index);
}

GuiCardSelector::NoSuitNoRank::NoSuitNoRank(GuiCardSelector* object) : GuiException(object) {
	object->Highlight();
}

GuiCardSelector::GuiCardSelector(QWidget* parent) : QWidget(parent) {
	CreateElements();
	PopulateSuits();
	PopulateRanks();
	CreateLayout();
	SelectData(this->suit, -1);
	SelectData(this->rank, -1);
}

GuiCardSelector::GuiCardSelector(const Config& config, QWidget* parent) : GuiCardSelector(parent) {
	SelectData(this->suit, config.suit);
	SelectData(this->rank, config.rank);
	inverse->setChecked(config.inversed);
}

void GuiCardSelector::CreateElements() {
	suit = new QComboBox();
	rank = new QComboBox();
	inverse = new QCheckBox(tr("Inverse"));
}

class Populater {
public:
	Populater(QComboBox* box) : box(box) {}
	void Add(const QString& title, int value) {
		box->addItem(title, value);
	}
private:
	QComboBox* box;
};

void GuiCardSelector::PopulateSuits() {
	Populater p(suit);
	p.Add(tr("Any suit"), -1);
	p.Add(tr("Diamond"), standard_36_deck::Suits::Diamonds);
	p.Add(tr("Spade"), standard_36_deck::Suits::Spades);
	p.Add(tr("Heart"), standard_36_deck::Suits::Hearts);
	p.Add(tr("Clubs"), standard_36_deck::Suits::Clubs);
}

void GuiCardSelector::PopulateRanks() {
	Populater p(rank);
	p.Add(tr("Any rank"), -1);
	p.Add(tr("Six"), standard_36_deck::Ranks::Six);
	p.Add(tr("Seven"), standard_36_deck::Ranks::Seven);
	p.Add(tr("Eight"), standard_36_deck::Ranks::Eight);
	p.Add(tr("Nine"), standard_36_deck::Ranks::Nine);
	p.Add(tr("Ten"), standard_36_deck::Ranks::Ten);
	p.Add(tr("Jack"), standard_36_deck::Ranks::Jack);
	p.Add(tr("Queen"), standard_36_deck::Ranks::Queen);
	p.Add(tr("King"), standard_36_deck::Ranks::King);
	p.Add(tr("Ace"), standard_36_deck::Ranks::Ace);
}

void GuiCardSelector::CreateLayout() {
	auto layout = new QVBoxLayout();
	layout->addWidget(suit);
	layout->addWidget(rank);
	layout->addWidget(inverse);

	QPushButton *btn = new QPushButton(tr("Remove"));
	layout->addWidget(btn);
	QObject::connect(btn, &QPushButton::clicked, this, &GuiCardSelector::DeleteClicked);

	frame = new QFrame();
	frame->setLayout(layout);
	frame->setFrameStyle(QFrame::StyledPanel);
	
	setLayout(new QGridLayout());
	this->layout()->addWidget(frame);
}

CardSelector GuiCardSelector::GetSelector() {
	int suitData = suit->currentData().toInt();
	int rankData = rank->currentData().toInt();

	bool suitSelected = suitData != -1;
	bool rankSelected = rankData != -1;
	bool isStraight = !inverse->isChecked();

	if (suitSelected && rankSelected) {
		return CardSelector(suitData, rankData, isStraight);
	} else if (suitSelected) {
		return CardSelector(Card::Suit(suitData), isStraight);
	} else if (rankSelected) {
		return CardSelector(Card::Rank(rankData), isStraight);
	} else {
		throw NoSuitNoRank(this);
	}
}

void GuiCardSelector::Highlight() {
	int style = frame->frameStyle();
	int width = frame->lineWidth();
	frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	frame->setLineWidth(3);

	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	QTimer::connect(timer, &QTimer::timeout, [style, width, timer, this](){
			frame->setFrameStyle(style);
			frame->setLineWidth(width);
			timer->deleteLater();
			});
	timer->start(5000);
}

