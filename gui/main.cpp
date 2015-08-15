#include <QApplication>
#include <QTimer>
#include <QSettings>

#include <windows/main_form.h>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setOrganizationName("mexus");
    app.setApplicationName("medici-gui");

    MainForm w;
    w.show();

    return app.exec();
}
