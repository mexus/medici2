#ifndef GUI_MAIN_FORM_H
#define	GUI_MAIN_FORM_H
#include <QMainWindow>
#include <logxx/logxx.h>

class MainForm : public QMainWindow {
        Q_OBJECT
public:
        MainForm(QWidget *parent = 0);
        virtual ~MainForm();
        
protected:
        virtual void closeEvent(QCloseEvent*);
        
        static logxx::Log cLog;

};

#endif	/* GUI_MAIN_FORM_H */

