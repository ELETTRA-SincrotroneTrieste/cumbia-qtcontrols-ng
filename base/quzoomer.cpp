#include "quzoomer.h"
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsObject>
#include <QtDebug>
#include <qgraphicssceneevent.h>
#include <QGraphicsSceneMouseEvent>

QuZoomer::QuZoomer(QObject *parent)
    : QObject{parent} {
    parent->installEventFilter(this);
}

bool QuZoomer::eventFilter(QObject *watched, QEvent *event) {
    bool ret = false;
    Qt::MouseButton butt;
    Qt::KeyboardModifiers m;
    bool graphicss = event->type() == QEvent::GraphicsSceneMousePress ||
                     event->type() == QEvent::GraphicsSceneMouseRelease ||
                     event->type() == QEvent::GraphicsSceneMouseMove;
    bool press = ( (event->type() == QEvent::GraphicsSceneMousePress) || (event->type() == QEvent::GraphicsSceneMousePress));
    bool release = ((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::MouseButtonRelease));
    bool move = ((event->type() == QEvent::GraphicsSceneMouseMove) || (event->type() == QEvent::MouseMove));
    QPointF pos;
    if(press || release || move) {
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
        qDebug() << __PRETTY_FUNCTION__ << "modif" << m << "butt" << butt << pos << "is graphicsscene" << graphicss;
        if((m & Qt::ShiftModifier) && (butt == Qt::LeftButton  || butt == Qt::MiddleButton || !p1.isNull())) {
            if(press && butt == Qt::LeftButton) {
                p1 = p2 = pos;
            }
            else if(move &&  !p1.isNull()) {
                p2 = pos;
            }

            m_update(watched);
            ret = true; // stop handling event further
        }

        if(release && butt == Qt::LeftButton && !p1.isNull() && !p2.isNull() && p1 != p2) {
            m_zstack.append(QRectF(p1, p2));
            p1 = p2 = QPointF();
            m_update(watched);
            ret = true;
        }
        else if(release && butt == Qt::MiddleButton && m_zstack.size() > 0) {
            m_zstack.pop_back();
        }
    }


    return ret ? ret : QObject::eventFilter(watched, event);
}

QRectF QuZoomer::zoomRect() const
{
    return m_zstack.size() > 0 ? m_zstack.last() : QRectF();
}

bool QuZoomer::inZoom() const
{
    return m_zstack.size() > 0;
}

int QuZoomer::stackSize() const {
    return m_zstack.size();
}

void QuZoomer::m_update(QObject *o)
{
    if(qobject_cast<QWidget *>(o))
            qobject_cast<QWidget *>(o)->update();
    else if(qobject_cast<QGraphicsObject *>(o))
            qobject_cast<QGraphicsObject *>(o)->update();
}
