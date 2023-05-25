#include "quzoomevents.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QtDebug>
#include <cumacros.h>

class QuZoomEventsP {
public:
    QuZoomEventsP() : modifiers{Qt::ShiftModifier, Qt::NoModifier},
        moving_view{false},
        zoom_level{0} {

    }

    Qt::KeyboardModifier modifiers[2];
    bool moving_view;
    int zoom_level;
};

QuZoomEvents::QuZoomEvents(QObject *parent)
    : QObject{parent}, d(new QuZoomEventsP){
    parent->installEventFilter(this);
}

QuZoomEvents::~QuZoomEvents() {
    delete d;
}

bool QuZoomEvents::eventFilter(QObject *watched, QEvent *event) {
    bool ret = false;
    Qt::MouseButton butt;
    Qt::KeyboardModifiers m;
    bool graphicss = event->type() == QEvent::GraphicsSceneMousePress ||
                     event->type() == QEvent::GraphicsSceneMouseRelease ||
                     event->type() == QEvent::GraphicsSceneMouseMove;
    bool press = ( (event->type() == QEvent::GraphicsSceneMousePress) || (event->type() == QEvent::MouseButtonPress));
    bool release = ((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::MouseButtonRelease));
    bool move = ((event->type() == QEvent::GraphicsSceneMouseMove) || (event->type() == QEvent::MouseMove));
    QPointF pos;

    if((press || release || move) && (d->modifiers[0] == d->modifiers[1])) {
        perr("QuZoomEvents.eventFilter: keyboard modifiers for defining zoom area and moving zoom area must be different");
    }
    else if(press || release || move) {
        if(graphicss) {
            QGraphicsSceneMouseEvent *sme = static_cast<QGraphicsSceneMouseEvent *>(event);
            butt = sme->button();
            m = sme->modifiers();
            pos = sme->pos();
        }
        else {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            butt = me->button();
            m = me->modifiers();
            pos = me->pos();
        }
        qDebug() << __PRETTY_FUNCTION__ << "modif" << m << "butt" << butt
                 << pos << "is graphicsscene" << graphicss <<
            "press" << press << "release" << release << "move" << move << "p1" << p1
                 << "p2" << p2;

        if((m & d->modifiers[Zoom] || d->modifiers[Zoom] == Qt::NoModifier) && (butt == Qt::LeftButton  || butt == Qt::MiddleButton || !p1.isNull())) {
            if(press && butt == Qt::LeftButton) {
                p1 = p2 = pos;
            }
            else if(move &&  !p1.isNull()) {
                p2 = pos;
                emit zoomRectChanging(p1, p2);
            }
            ret = true; // stop handling event further
        }
        else if((m & d->modifiers[Move] || d->modifiers[Move] == Qt::NoModifier) && d->zoom_level > 0 && press) {
            d->moving_view = true;
            p1 = pos;
        }
        else if(d->moving_view && move) {
            p2 = pos;
            emit moveRect(p1, p2);
            p1 = p2;
            ret = true;
        }
        else if(d->moving_view && release) {
            p1 = p2 = QPointF();
            d->moving_view = false;
        }

        if(release && butt == Qt::LeftButton && !p1.isNull() && !p2.isNull() && p1 != p2) {
            d->zoom_level++;
            emit zoomRectChanged(QRectF(p1, p2));
            emit zoomRectChanging(QPointF(), QPointF()); // mouse released, stop drawing
            p1 = p2 = QPointF();
            ret = true;
        }
        else if(release && butt == Qt::MiddleButton && d->zoom_level > 0) {
            d->zoom_level--;
            emit unzoom();
            ret = true;
        }
    }
    return ret ? ret : QObject::eventFilter(watched, event);
}

void QuZoomEvents::setKeyboardModifier(Operation op, Qt::KeyboardModifier mod) {
    if(op < EndOps)
        d->modifiers[op] = mod;
}

Qt::KeyboardModifier QuZoomEvents::keyboardModifier(Operation op) const {
    return (op < EndOps) ? d->modifiers[op] : Qt::NoModifier;
}
