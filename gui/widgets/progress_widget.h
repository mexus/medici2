#ifndef GUI_WIDGETS_PROGRESS_WIDGET_H
#define GUI_WIDGETS_PROGRESS_WIDGET_H

#include <QGroupBox>
#include <QLabel>

#include <calculator/thread.h>

class ProgressWidget : public QGroupBox {
    Q_OBJECT
public:
    ProgressWidget(std::uint8_t threadNumber, QWidget *parent = 0);
    void Set(const calculator::Thread::RunParameters&);
private:
    QLabel *totalDecks, *suitableDecks, *runningTime;

    void CreateObjects();
    void CreateLayout();
};

#endif /* GUI_WIDGETS_PROGRESS_WIDGET_H */
