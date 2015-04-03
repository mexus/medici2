#ifndef GUI_WIDGETS_CLICKABLE_LABEL_H
#define GUI_WIDGETS_CLICKABLE_LABEL_H

#include <QGroupBox>

class QMouseEvent;

class ClicableGroupBox : public QGroupBox {
    Q_OBJECT
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
signals:
    void doubleClicked();
};

#endif /* GUI_WIDGETS_CLICKABLE_LABEL_H */
