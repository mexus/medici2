#ifndef GUI_MAIN_FORM_H
#define	GUI_MAIN_FORM_H
#include <QMainWindow>
#include <QTabWidget>


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

	void AddSelectorTab(const QString& label);
	void RenameSelector(int index);
        
        static logxx::Log cLog;

};

#endif	/* GUI_MAIN_FORM_H */

