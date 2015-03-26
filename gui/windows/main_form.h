#ifndef GUI_WINDOWS_MAIN_FORM_H
#define GUI_WINDOWS_MAIN_FORM_H
#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QSettings>
#include <QJsonArray>
#include <logxx/logxx.h>
#include <selectors/gui_deck_selector.h>
#include "calculator_window.h"
#include <widgets/deck_preference.h>
#include <helpers/cards_translations.h>


class MainForm : public QMainWindow {
    Q_OBJECT
public:
    MainForm(QWidget *parent = 0);
    virtual ~MainForm();
        
protected:
    virtual void closeEvent(QCloseEvent*);
    CardsTranslations cardsTranslations;

    QTabWidget* tabs;
    QPushButton *actionButton;
    CalculatorWindow *calculator;
    DeckPreference *deckPreferenceTab;

    void CreateObjects(const QJsonObject&);
    void CreateLayout();

    void AddSelectorTab(GuiDeckSelector*, const QString& label);
    void AddSelectorTab();
    void RenameSelector(int index);
        
    static logxx::Log cLog;

    void LoadSelectorTabs(const QJsonArray&);
    QJsonArray SaveSelectorTabs() const;

    DeckSelectors GetSelectors();
    void ActivateCalculation();

};

#endif  /* GUI_WINDOWS_MAIN_FORM_H */
