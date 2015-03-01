#include "main_form.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>

logxx::Log MainForm::cLog("MainForm");

void MainForm::AddSelectorTab(const QString& label) {
	auto selector = new GuiDeckSelector();
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
        S_LOG("MainForm");
        QSettings settings;
        log(logxx::debug) << "Restoring geometry and state" << logxx::endl;
        restoreGeometry(settings.value("main-window:geometry").toByteArray());
        restoreState(settings.value("main-window:state").toByteArray());

	setCentralWidget(new QWidget());
	auto layout = new QVBoxLayout();

	layout->addWidget(new QPushButton(tr("Add conditions set")));

	tabs = new QTabWidget();
	AddSelectorTab(tr("Test"));
	AddSelectorTab(tr("Test2"));
	AddSelectorTab(tr("Test3"));
	AddSelectorTab(tr("Test4"));
	QObject::connect(tabs, &QTabWidget::tabBarDoubleClicked, this, &MainForm::RenameSelector);

	layout->addWidget(tabs);

	centralWidget()->setLayout(layout);
}

MainForm::~MainForm() {
}

void MainForm::closeEvent(QCloseEvent*) {
        S_LOG("closeEvent");
        QSettings settings;
        log(logxx::debug) << "Saving geometry and state" << logxx::endl;
        settings.setValue("main-window:geometry", saveGeometry());
        settings.setValue("main-window:state", saveState());
}

