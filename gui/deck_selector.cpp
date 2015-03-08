#include "deck_selector.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GuiDeckSelector::Config GuiDeckSelector::GetConfig() const {
	std::vector<GuiCardSelector::Config> cards;
	for (auto &guiCardSelector: selectors) {
		cards.push_back(std::move(guiCardSelector->GetConfig()));
	}
	return {
		selectorMode->currentData().toInt(),
		static_cast<std::size_t>(positionStart->value()),
		static_cast<std::size_t>(positionEnd->value()),
		enabled->isChecked(),
		std::move(cards)
	};
}

GuiDeckSelector::GuiDeckSelector(QWidget *parent) : QWidget(parent) {
	CreateElements();
	SetSpinBoxes();
	CreateLayout();
}

GuiDeckSelector::GuiDeckSelector(const Config& config, QWidget *parent) : GuiDeckSelector(parent) {
	auto index = selectorMode->findData(config.selectorMode);
	if (index != -1)
		selectorMode->setCurrentIndex(index);
	positionStart->setValue(config.positionBegin);
	positionEnd->setValue(config.positionEnd);
	enabled->setChecked(config.enabled);
	for (auto& cardConfig : config.cards) {
		AddCardSelector(cardConfig);
	}
}

void GuiDeckSelector::AddCardSelector() {
	AddCardSelector(new GuiCardSelector());
}

void GuiDeckSelector::AddCardSelector(const GuiCardSelector::Config& config) {
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

