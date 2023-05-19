#include "quzoomer.h"
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsObject>
#include <QtDebug>
#include <qgraphicssceneevent.h>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <qpen.h>
#include <qpainter.h>
#include <cumacros.h>

class QuZoomerP {
public:
    QuZoomerP() : moving_view(false) {}

    bool moving_view;
    QVector<QRectF> m_zstack;
    QTransform m_transform;
};

QuZoomer::QuZoomer(QObject *parent)
    : QObject{parent}, d(new QuZoomerP) {
    parent->installEventFilter(this);
}

QuZoomer::~QuZoomer() {
    delete d;
}

bool QuZoomer::eventFilter(QObject *watched, QEvent *event) {
//    qDebug() << __PRETTY_FUNCTION__ << watched << event;
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
    if(press || release || move) {
        if(graphicss) {
            QGraphicsSceneMouseEvent *sme = static_cast<QGraphicsSceneMouseEvent *>(event);
            butt = sme->button();
            m = sme->modifiers();
            pos = m_map_from_pos(sme->pos());
        }
        else {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            butt = me->button();
            m = me->modifiers();
            pos = m_map_from_pos(me->pos());
        }
        qDebug() << __PRETTY_FUNCTION__ << "modif" << m << "butt" << butt
                 << pos << "is graphicsscene" << graphicss <<
            "press" << press << "release" << release << "move" << move << "p1" << p1
                 << "p2" << p2;

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
        else if(d->m_zstack.size() > 0 && press) {
            d->moving_view = true;
            p1 = pos;
        }
        else if(d->moving_view && move) {
            p2 = pos;
            m_move_zooms();
            p1 = p2;
            qDebug() << __PRETTY_FUNCTION__ << "moving zooms";
            m_update(watched);
        }
        else if(d->moving_view && release) {
            p1 = p2 = QPointF();
            d->moving_view = false;
        }

        if(release && butt == Qt::LeftButton && !p1.isNull() && !p2.isNull() && p1 != p2) {
            m_add_zoom(QRectF(p1, p2));
            p1 = p2 = QPointF();
            m_update(watched);
            ret = true;
        }
        else if(release && butt == Qt::MiddleButton && d->m_zstack.size() > 0) {
            d->m_zstack.pop_back();
            if(!d->m_zstack.size())
                d->m_transform = QTransform();
            m_update(watched);
        }
    }
    return ret ? ret : QObject::eventFilter(watched, event);
}

QRectF QuZoomer::zoomRect() const
{
    return d->m_zstack.size() > 0 ? d->m_zstack.last() : QRectF();
}

bool QuZoomer::inZoom() const
{
    return d->m_zstack.size() > 0;
}

int QuZoomer::stackSize() const {
    return d->m_zstack.size();
}

bool QuZoomer::drawZoomRect(QPainter *p, const QRectF &r, QWidget *widget) {
    if(d->m_zstack.size() > 0) {
        zoom(p, r, widget);
    }
    QPen pe(Qt::black, 0.0);
    p->setPen(pe);
    p->drawRect(QRectF(p1, p2));
    QFont f = p->font();
    f.setPointSize(10);
    p->setFont(f);
    p->drawText(QRectF(p1, p2).center(), QString("%1x%2").arg(r.width(), r.height()));
    return true; // stop processing
}

bool QuZoomer::zoom(QPainter *p, const QRectF &r, QWidget *widget) {
    if(d->m_zstack.size() > 0) {
        p->setWindow(r.toRect());
        float ratio;
        float dx, dy, ddx = 0, ddy = 0;
        float m = qMin(r.width(), r.height()); // smaller side
        const QRectF& zr = d->m_zstack[d->m_zstack.size() - 1];
        if(!r.contains(zr))
            printf("\e[1;31mWARNING zoom rect bigger than painter rect\e[0m!!\n");
        if(zr.width() > zr.height()) {
            ratio = zr.width() / m; // < 1
//            zr.setHeight(zr.height() / ratio);
            // in case of wide and short rectangles, adjust Y so that the viewport is centered
            ddy = r.height() / 2.0 - zr.height() / ratio / 2.0;
        }
        else {
            ratio = zr.height() / m;  // < 1
//            zr.setWidth(zr.width() / ratio);
            // in case of high and slim rectangles, adjust X so that the viewport is centered
            ddx = r.width() / 2.0 - zr.width() / ratio / 2.0;
        }
        dx = zr.x() / ratio;
        dy = zr.y() / ratio;
        QRectF za(-dx + ddx, -dy + ddy, r.width() / ratio, r.height() / ratio);

//        qDebug() << __PRETTY_FUNCTION__ << "dx" << dx << "dy" << dy;
        qDebug() << __PRETTY_FUNCTION__ << "zoom rect" << zr << "--> area " << za
                 << "ddx" << ddx << "ddy" << ddy << "painter rect" << r;
//        qDebug() << __PRETTY_FUNCTION__ << "painter w / zoom rect w " << r.width() / zr.width()  <<
//            "painter h / zoom rect h" << r.height() / zr.height();
        // r e zr are now in   scale

//        p->translate(-dx + ddx, -dy + ddy);
//        p->scale(1.0/ratio, 1.0/ratio);

        p->setViewport(za.toRect());
        d->m_transform = p->deviceTransform();
//        qDebug() << __PRETTY_FUNCTION__ << "transform" << d->m_transform << "Inverted" << d->m_transform.inverted()
//                 << "device Transform" << devT << "combined Transform" << cT;
    }
    return false;
}

void QuZoomer::m_update(QObject *o)
{
    if(qobject_cast<QWidget *>(o))
        qobject_cast<QWidget *>(o)->update();
    else if(qobject_cast<QGraphicsObject *>(o))
        qobject_cast<QGraphicsObject *>(o)->update();
}

void QuZoomer::m_move_zooms() {
    for(int i = 0; i < d->m_zstack.size(); i++) {
        const float dx = p2.x() - p1.x(), dy = p2.y() - p1.y();
        qDebug() << "moving zooms by " << dx << "," << dy;
        d->m_zstack[i].moveLeft(d->m_zstack[i].left() - dx);
        d->m_zstack[i].moveTop(d->m_zstack[i].top() - dy);
    }
}

void QuZoomer::m_add_zoom(const QRectF &area) {
        d->m_zstack.append(QRectF(area));
}

QPointF QuZoomer::m_map_from_pos(const QPointF &pt) {
    QPointF pxf = d->m_transform.inverted().map(pt);
    qDebug() << __PRETTY_FUNCTION__ << "from" << pt << "to" << pxf;
    return pxf;
}

QPainter QuZoomer::m_get_painter(QObject *o) const {
    QWidget * w = qobject_cast<QWidget *>(o);
    if(w)
        return QPainter(w);
    else {
        //        QGraphicsObject *go = qobject_cast<QGraphicsObject *>(o);
        //        if(go)
        //            return QPainter(static_cast<QGraphicsItem *>(go));
    }
    return QPainter();
}
