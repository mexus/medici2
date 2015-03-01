#include "card_selector.h"
#include <QLayout>
#include <QVBoxLayout>
#include <cards/standard-36-deck.h>
#include <QPushButton>
#include <type_traits>
#include <exception>

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

void GuiCardSelector::PopulateSuits() {
	suit->addItem(tr("None"), (int)-1);
	suit->addItem(tr("Diamond"), (int)standard_36_deck::Suits::Diamonds);
	suit->addItem(tr("Spade"), (int)standard_36_deck::Suits::Spades);
	suit->addItem(tr("Heart"), (int)standard_36_deck::Suits::Hearts);
	suit->addItem(tr("Clubs"), (int)standard_36_deck::Suits::Clubs);
}

void GuiCardSelector::PopulateRanks() {
	rank->addItem(tr("None"), (int)-1);
	rank->addItem(tr("Six"), (int)standard_36_deck::Ranks::Six);
	rank->addItem(tr("Seven"), (int)standard_36_deck::Ranks::Seven);
	rank->addItem(tr("Eight"), (int)standard_36_deck::Ranks::Eight);
	rank->addItem(tr("Nine"), (int)standard_36_deck::Ranks::Nine);
	rank->addItem(tr("Ten"), (int)standard_36_deck::Ranks::Ten);
	rank->addItem(tr("Jack"), (int)standard_36_deck::Ranks::Jack);
	rank->addItem(tr("Queen"), (int)standard_36_deck::Ranks::Queen);
	rank->addItem(tr("King"), (int)standard_36_deck::Ranks::King);
	rank->addItem(tr("Ace"), (int)standard_36_deck::Ranks::Ace);
}

void GuiCardSelector::CreateLayout() {
	auto layout = new QVBoxLayout();
	layout->addWidget(suit);
	layout->addWidget(rank);
	layout->addWidget(inverse);
	setLayout(layout);

	QPushButton *btn = new QPushButton();
	layout->addWidget(btn);
	QObject::connect(btn, &QPushButton::clicked, this, &GuiCardSelector::DeleteClicked);
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

