#include "clickable_group_box.h"
#include <QMouseEvent>

void ClicableGroupBox::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
        emit doubleClicked();
    QGroupBox::mouseDoubleClickEvent(e);
}

