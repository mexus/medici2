#ifndef GUI_WIDGETS_CLICKABLE_GROUP_BOX_H
#define GUI_WIDGETS_CLICKABLE_GROUP_BOX_H

#include <QGroupBox>

class QMouseEvent;

class ClicableGroupBox : public QGroupBox {
    Q_OBJECT
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
signals:
    void doubleClicked();
};

#endif /* GUI_WIDGETS_CLICKABLE_GROUP_BOX_H */
