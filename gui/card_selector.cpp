#include "card_selector.h"
#include <QLayout>
#include <QVBoxLayout>
#include <cards/standard-36-deck.h>
#include <QPushButton>
#include <type_traits>
#include <exception>

#include <QFrame>

template<class T>
typename std::enable_if<std::is_same<T, QComboBox>::value>::type SelectData(T* comboBox, int data) {
	int index = comboBox->findData(data);
	if (index == -1)
		throw std::runtime_error("Unknown data");
	comboBox->setCurrentIndex(index);
}

GuiCardSelector::GuiCardSelector(int suit, int rank, bool inversed, QWidget* parent) : QWidget(parent) {
	CreateElements();
	PopulateSuits();
	PopulateRanks();
	CreateLayout();

	SelectData(this->suit, suit);
	SelectData(this->rank, rank);
	inverse->setChecked(inversed);
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

	auto frame = new QFrame();
	frame->setLayout(layout);
	frame->setFrameStyle(QFrame::StyledPanel);
	
	setLayout(new QGridLayout());
	this->layout()->addWidget(frame);
}

CardSelector GuiCardSelector::GetSelector() const {
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
		throw std::runtime_error("No suit or rank selected");
	}
}

