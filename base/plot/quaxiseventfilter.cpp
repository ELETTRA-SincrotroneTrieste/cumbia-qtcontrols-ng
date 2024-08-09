#include "quaxiseventfilter.h"
#include <QEvent>

QuAxisEventFilter::QuAxisEventFilter(QObject *parent)
    : QObject{parent}
{}

bool QuAxisEventFilter::eventFilter(QObject *o, QEvent *e) {
    if(e->type() == QEvent::MouseButtonDblClick) {
        emit doubleClicked();
        return true;
    } else if (e->type() == QEvent::ContextMenu) {
        emit contextMenuRequest();
        return true;
    }
    return QObject::eventFilter(o, e);
}

