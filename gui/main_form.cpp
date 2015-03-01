#include "main_form.h"

#include <type_traits>

#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>

#include "settings_helper.h"

logxx::Log MainForm::cLog("MainForm");

void MainForm::AddSelectorTab(GuiDeckSelector* selector, const QString& label) {
	tabs->addTab(selector, label);

	QObject::connect(selector, &GuiDeckSelector::DeleteClicked, [this, selector](){
			if (tabs->count() != 1) {
				auto index = tabs->indexOf(selector);
				tabs->removeTab(index);
				selector->deleteLater();
			}
			});
}

void MainForm::RenameSelector(int index) {
	auto selector = dynamic_cast<GuiDeckSelector*>(tabs->widget(index));
	if (selector) {
		bool ok;
		QString text = QInputDialog::getText(this, tr("Conditions name"),
				tr("Condition name:"), QLineEdit::Normal,
				tabs->tabText(index), &ok);
		if (ok && !text.isEmpty())
		tabs->setTabText(index, text);
	}
}

MainForm::MainForm(QWidget* parent) : QMainWindow(parent) {
        QSettings settings;
	{
		SettingsGetHelper helper(settings, "main-window:");
	        restoreGeometry(helper("geometry").toByteArray());
	        restoreState(helper("state").toByteArray());
	}

	setCentralWidget(new QWidget());
	auto layout = new QVBoxLayout();

	layout->addWidget(new QPushButton(tr("Add conditions set")));

	tabs = new QTabWidget();
	LoadSelectorTabs(settings);
	QObject::connect(tabs, &QTabWidget::tabBarDoubleClicked, this, &MainForm::RenameSelector);

	layout->addWidget(tabs);

	centralWidget()->setLayout(layout);
}

MainForm::~MainForm() {
}

void MainForm::closeEvent(QCloseEvent*) {
	QSettings settings;
	{
		SettingsSetHelper helper(settings, "main-window:");
	        helper("geometry", saveGeometry());
	        helper("state", saveState());
	}
	SaveSelectorTabs(settings);
}

void MainForm::LoadSelectorTabs(const QSettings& settings) {
	SettingsGetHelper helper(settings);
	std::size_t conditionsCount = helper("conditions-count").toUInt();
	if (conditionsCount == 0)
		AddSelectorTab(new GuiDeckSelector(), tr("First condition"));
	for (std::size_t i = 0; i != conditionsCount; ++i) {
		SettingsGetHelper conditionHelper(helper, "condition-tab-" + QString::number(i) + ":");
		GuiDeckSelector::Config config;
		config.selectorMode = conditionHelper("selector-mode").toInt();
		config.positionBegin = conditionHelper("position-begin").toUInt();
		config.positionEnd = conditionHelper("position-end").toUInt();
		config.enabled = conditionHelper("enabled").toBool();
		QString label = conditionHelper("label").toString();

		std::size_t cardsCount = conditionHelper("cards-count").toUInt();
		for (std::size_t j = 0; j != cardsCount; ++j) {
			SettingsGetHelper cardHelper(conditionHelper, "card-" + QString::number(j) + "-");
			config.cards.push_back({
				cardHelper("suit").toInt(),
				cardHelper("rank").toInt(),
				cardHelper("inversed").toBool()
			});
		}
		AddSelectorTab(new GuiDeckSelector(config), label);
	}
}

void MainForm::SaveSelectorTabs(QSettings& settings) {
	SettingsSetHelper helper(settings);
	std::size_t conditionsCount = tabs->count();
	helper("conditions-count", (unsigned int)conditionsCount);
	for (std::size_t i = 0; i != conditionsCount; ++i) {
		SettingsSetHelper conditionHelper(helper, "condition-tab-" + QString::number(i) + ":");
		auto config = static_cast<GuiDeckSelector*>(tabs->widget(i))->GetConfig();
		conditionHelper("selector-mode", config.selectorMode);
		conditionHelper("position-begin", (unsigned int)config.positionBegin);
		conditionHelper("position-end", (unsigned int)config.positionEnd);
		conditionHelper("enabled", config.enabled);
		conditionHelper("cards-count", (unsigned int)config.cards.size());
		conditionHelper("label", tabs->tabText(i));

		std::size_t j(0);
		for (auto &cardConfig: config.cards) {
			SettingsSetHelper cardHelper(conditionHelper, "card-" + QString::number(j) + "-");
			cardHelper("suit", cardConfig.suit);
			cardHelper("rank", cardConfig.rank);
			cardHelper("inversed", cardConfig.inversed);
			++j;
		}
	}
	
}


