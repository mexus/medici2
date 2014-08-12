#include "main_form.h"
#include <QSettings>

logxx::Log MainForm::cLog("MainForm");

MainForm::MainForm(QWidget* parent) : QMainWindow(parent) {
        S_LOG("MainForm");
        QSettings settings;
        log(logxx::debug) << "Restoring geometry and state" << logxx::endl;
        restoreGeometry(settings.value("main-window:geometry").toByteArray());
        restoreState(settings.value("main-window:state").toByteArray());
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

