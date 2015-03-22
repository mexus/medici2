#ifndef GUI_MAIN_FORM_H
#define GUI_MAIN_FORM_H
#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QSettings>
#include <QJsonArray>
#include <logxx/logxx.h>
#include "selectors/gui_deck_selector.h"
#include "calculator_window.h"
#include "deck_preference.h"


class MainForm : public QMainWindow {
    Q_OBJECT
public:
    MainForm(QWidget *parent = 0);
    virtual ~MainForm();
        
protected:
    virtual void closeEvent(QCloseEvent*);

    QTabWidget* tabs;
    QPushButton *actionButton;
    CalculatorWindow *calculator;
    DeckPreference *deckPreferenceTab;

    void CreateObjects(const QJsonObject&);
    void CreateLayout();

    void AddSelectorTab(GuiDeckSelector* = new GuiDeckSelector(), const QString& label = tr("New"));
    void RenameSelector(int index);
        
    static logxx::Log cLog;

    void LoadSelectorTabs(const QJsonArray&);
    QJsonArray SaveSelectorTabs() const;

    DeckSelectors GetSelectors();
    void ActivateCalculation();

};

#endif  /* GUI_MAIN_FORM_H */

