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
    QRectF zoom_area, current_rect;
    QRect viewport;
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
        const QTransform& t = d->T;

        qDebug() << __PRETTY_FUNCTION__ << p1 << p2 << "TRANSFORM" << d->T;
        // divide by the scale factors t.m11 and t.m22 to keep the movement anchored to the mouse
        const float dx = (p2.x()  - p1.x()) / t.m11(), dy = (p2.y() - p1.y()) / t.m22();
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

/*!
 * \brief notify the zoomer that the monitored object (QWidget, QGraphicsItem) has changed
 *        its size.
 * \param newSiz the new size
 *
 * If the attached object changes size (e.g. QWidget::resizeEvent), the zoom rect(s) shall be
 * resized and repositioned so that the viewport stays centered with the content being viewed
 */
void QuZoomer::sizeChanged(const QSize &newSiz) {
    float dx = (newSiz.width() - d->current_rect.width());
    float dy = (newSiz.height() - d->current_rect.height());

    const QPointF &cc = d->current_rect.center();
//    moveZoom(cc, QPointF(cc.x() - dx, cc.y() - dy));

    float wr = newSiz.width() / d->current_rect.width();
    float hr = newSiz.height() / d->current_rect.height();

    for(int i = 0; i < d->m_zstack.size(); i++) {
        // divide by the scale factors t.m11 and t.m22 to keep the movement anchored to the mouse

        QRectF oldr = d->m_zstack[i];
        QRectF& r = d->m_zstack[i];
//        QPointF center = d->T.map(d->viewport.center());
//        r.moveLeft(r.left() - dx);
//        r.moveTop(r.top() - dy);
//        r.moveCenter(center);
        r.setWidth(r.width() / wr);  // scale width
        r.setHeight(r.height() / hr);  // scale height

//        qDebug() << __PRETTY_FUNCTION__ << "moving by " << dx << dy
//                 << "current rect " << d->current_rect.size() << "new rect" << newSiz
//                 << d->T << "rect top " << oldr.top() << " Y moved to"
//                 << r.top();

        emit zoomChanged();
    }
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

const QRect &QuZoomer::setViewport( QPainter * painter, const QRectF& r) const {
    d->current_rect = r;
    // if zoom rect changed recalculate the viewport
    // otherwise return the current viewport
    if(d->m_zstack.size() > 0/* && d->changed*/) {
        float ratio;
        float dx, dy, ddx = 0, ddy = 0;
        float m = qMin(r.width(), r.height()); // smaller side
        const QRectF& zr = d->m_zstack[d->m_zstack.size() - 1];
        float M = qMax(zr.width(), zr.height());
        if(!r.contains(zr) /*&& !d->changed*/) {
            printf("\e[1;31mWARNING zoom rect OUTSIDE painter rect\e[0m!!\n");
            qDebug() << __PRETTY_FUNCTION__ << "painter rect" << r << "zoom rect" << zr;
        }
        /*else*/ {
            ratio = M / m;
            if(zr.width() > zr.height()) {
//                ratio = zr.width() / m; // < 1
                //            zr.setHeight(zr.height() / ratio);
                // in case of wide and short rectangles, adjust Y so that the viewport is centered
                ddy = r.height() / 2.0 - zr.height() / ratio / 2.0;
            }
            else {
//                ratio = zr.height() / m;  // < 1
                //            zr.setWidth(zr.width() / ratio);
                // in case of high and slim rectangles, adjust X so that the viewport is centered
                ddx =  r.width() / 2.0 - zr.width() / ratio / 2.0;
            }
            if(ratio > 1 )
                ratio = 1;
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

            d->viewport = za.toRect();
            //        qDebug() << __PRETTY_FUNCTION__ << "transform" << d->m_transform << "Inverted" << d->m_transform.inverted()
            //                 << "device Transform" << devT << "combined Transform" << cT;
        }
        painter->setViewport(d->viewport);
        d->T = painter->deviceTransform();
    }
    else /*if(d->changed)*/ {
        qDebug() << __PRETTY_FUNCTION__ << "resetting transform and viewport";
        d->T = QTransform();
        d->viewport = r.toRect();
    }

    d->changed = false;
    return d->viewport;
}

void QuZoomer::m_add_zoom(const QRectF &area) {
    d->m_zstack.append(QRectF(area));
}

QPointF QuZoomer::m_map_from_pos(const QPointF &pt) {
    QPointF pxf = d->T.inverted().map(pt);
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
