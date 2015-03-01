#ifndef GUI_MAIN_FORM_H
#define	GUI_MAIN_FORM_H
#include <QMainWindow>
#include <QTabWidget>
#include <QSettings>

#include "deck_selector.h"

#include <logxx/logxx.h>

class MainForm : public QMainWindow {
        Q_OBJECT
public:
        MainForm(QWidget *parent = 0);
        virtual ~MainForm();
        
protected:
        virtual void closeEvent(QCloseEvent*);

	QTabWidget* tabs;

	void AddSelectorTab();
	void AddSelectorTab(GuiDeckSelector*, const QString& label);
	void RenameSelector(int index);
        
        static logxx::Log cLog;

	void LoadSelectorTabs(const QSettings&);
	void SaveSelectorTabs(QSettings&);

};

#endif	/* GUI_MAIN_FORM_H */

