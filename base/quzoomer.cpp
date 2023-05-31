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
    QuZoomerP() : changed(false) {}

    QVector<QRectF> m_zstack;
    QTransform T;
    QRectF zoom_area;
    bool changed;
};

QuZoomer::QuZoomer(QObject *parent)
    : QObject{parent}, d(new QuZoomerP) {
}

QuZoomer::~QuZoomer() {
    delete d;
}


QRectF QuZoomer::zoomRect() const {
    return d->m_zstack.size() > 0 ? d->m_zstack.last() : QRectF();
}

const QRectF &QuZoomer::zoomArea() const {
    return d->zoom_area;
}

void QuZoomer::setZoomRect(const QRectF &r) {
    qDebug() << __PRETTY_FUNCTION__ << r << "mapped" << d->T.mapRect(r);
    d->m_zstack.append(d->T.inverted().mapRect(r));
    d->changed = true;
    emit zoomChanged();
}

void QuZoomer::moveZoom(const QPointF &p1, const QPointF &p2) {
    for(int i = 0; i < d->m_zstack.size(); i++) {
//        qDebug() << __PRETTY_FUNCTION__ << p1 << p2 << "TRANSFORM" << d->T;
        // divide by the scale factors t.m11 and t.m22 to keep the movement anchored to the mouse
        const float dx = (p2.x()  - p1.x()) / d->T.m11(), dy = (p2.y() - p1.y()) / d->T.m22();
        d->m_zstack[i].moveLeft(d->m_zstack[i].left() - dx);
        d->m_zstack[i].moveTop(d->m_zstack[i].top() - dy);
    }
    d->changed = true;
    emit zoomChanged();
}

void QuZoomer::unzoom() {
    d->m_zstack.pop_back();
    if(!d->m_zstack.size())
        d->T = QTransform();
    d->changed = true;
    emit zoomChanged();
}

void QuZoomer::zoomRectChanging(const QPointF &topl, const QPointF &botr) {

    d->zoom_area = QRectF(d->T.inverted().map(topl), d->T.inverted().map(botr));
    emit zoomChanged();
}

void QuZoomer::map(const QPointF &p, const Qt::MouseButton butt, Qt::KeyboardModifiers mod) {
    emit clicked(d->T.inverted().map(p), butt, mod);
}

bool QuZoomer::inZoom() const {
    return d->m_zstack.size() > 0;
}

int QuZoomer::stackSize() const {
    return d->m_zstack.size();
}

bool QuZoomer::rectChanging() const {
    return d->zoom_area.isValid();
}

bool QuZoomer::viewportChanged() const {
    return d->changed;
}

const QTransform &QuZoomer::calculateTransform(const QRectF& r) const {
    // if zoom rect changed recalculate the viewport
    // otherwise return the current viewport
    if(d->m_zstack.size() > 0/* && d->changed*/) {
        float ratio;
        float dx, dy, ddx = 0, ddy = 0;
        const QRectF& zr = d->m_zstack[d->m_zstack.size() - 1];
        float m = qMin(r.width(), r.height()); // smaller side
        float M = qMax(zr.width(), zr.height());
        if(!r.contains(zr) /*&& !d->changed*/) {
            printf("\e[1;31mWARNING zoom rect OUTSIDE painter rect\e[0m!!\n");
        }
        ratio = M / m;
        if(ratio > 1 ) // do not scale beyond zoom rect
            ratio = 1;
        if(zr.width() > zr.height()) {
            // in case of wide and short rectangles, adjust Y so that the viewport is centered
            ddy = r.height() / 2.0 - zr.height() / ratio / 2.0;
        }
        else {
            // in case of high and slim rectangles, adjust X so that the viewport is centered
            ddx =  r.width() / 2.0 - zr.width() / ratio / 2.0;
        }
        dx = zr.x() / ratio;
        dy = zr.y() / ratio;
        float m31 =  -dx + ddx;
        float m32 = -dy + ddy;
        d->T = QTransform(1 / ratio,    0.0,                0.0,
                          0.0,          1 / ratio,          0.0,
                          m31,          m32,                1.0);
    }
    else /*if(d->changed)*/ {
        qDebug() << __PRETTY_FUNCTION__ << "resetting transform and viewport";
        d->T = QTransform();
    }
    d->changed = false;
    return d->T;
}

const QTransform &QuZoomer::transform() const {
    return d->T;
}

void QuZoomer::m_add_zoom(const QRectF &area) {
    d->m_zstack.append(QRectF(area));
}

QPointF QuZoomer::map(const QPointF &pt) {
    return d->T.inverted().map(pt);
}

QPainter QuZoomer::m_get_painter(QObject *o) const {
    QWidget * w = qobject_cast<QWidget *>(o);
    if(w)
        return QPainter(w);
    return QPainter();
}
