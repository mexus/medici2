#include "deck_selector.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GuiDeckSelector::GuiDeckSelector(QWidget *parent) : QWidget(parent) {
	CreateElements();
	SetSpinBoxes();
	CreateLayout();
}

void GuiDeckSelector::AddCardSelector(int suit, int rank, bool inversed) {
	auto selector = new GuiCardSelector(suit, rank, inversed);
	selectors.insert(selector);
	QObject::connect(selector, &GuiCardSelector::DeleteClicked, [this, selector](){
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
}

void GuiDeckSelector::SetSpinBoxes() {
	positionStart = CreateSpinBox(1, 36);
	positionEnd = CreateSpinBox(1, 36);
}

void GuiDeckSelector::CreateLayout() {
	auto mainLayout = new QVBoxLayout();
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
	std::unique_ptr<DeckAbstractSelector> selector;
	SelectorMode mode = static_cast<SelectorMode>(selectorMode->currentData().toUInt());

	std::vector<CardSelector> cardSelectors;
	for (auto &guiCardSelector: selectors) {
		cardSelectors.push_back(std::move(guiCardSelector->GetSelector()));
	}
	std::size_t from = positionStart->value();
	std::size_t to = positionEnd->value();

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

