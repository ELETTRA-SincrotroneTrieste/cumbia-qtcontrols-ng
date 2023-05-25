#include "qucircularplotengine.h"
#include "qucircularplotitem.h"
#include "quzoomer.h"

#include <cumacros.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

#include <quzoomer.h>
#include <quzoomevents.h>

class QuCircularPlotI_P {
public:
    QuCircularPlotI_P(QGraphicsObject *gobj, const QFont& f)
        : e(new QuCircularPlotEngine(f, new QuZoomer(gobj), new QuZoomEvents(gobj))), rect(0, 0, 100, 40) {
        // circular plot engine takes the ownership of zoomer and zoom events
    }
    ~QuCircularPlotI_P() {
        delete e;
    }
    QuCircularPlotEngine *e;
    QRectF rect;
};

QuCircularPlotI::QuCircularPlotI(QGraphicsItem *parent) : QGraphicsObject(parent) {
    d = new QuCircularPlotI_P(this, QFont());
    setAcceptHoverEvents(true); // mouse tracking
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    connect(d->e->zoomer(), SIGNAL(zoomChanged()), this, SLOT(update()));
}

QuCircularPlotI::~QuCircularPlotI() {
    delete d;
}

void QuCircularPlotI::update() {
//    if(d->e->zoomer()->inZoom() && scene()) {
//        printf("\e[1;32mQuCircularPlotI::update inZoom: updating scene\e[0m");
//        scene()->update();
//    }
//    else
    QGraphicsObject::update(QRectF());
}

void QuCircularPlotI::setData(const QString &src, const QVector<double> &xdata, const QVector<double> &ydata) {
    d->e->setData(src, xdata, ydata);
    update();
}

QuCircularPlotEngine *QuCircularPlotI::engine() const {
    return d->e;
}


QRectF QuCircularPlotI::boundingRect() const {
    return d->e->d.bounding_r;
}

void QuCircularPlotI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    d->e->paint(painter, option->rect, widget);
}

void QuCircularPlotI::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void QuCircularPlotI::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        update();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void QuCircularPlotI::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsObject::hoverMoveEvent(event);
}

void QuCircularPlotI::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsObject::hoverLeaveEvent(event);
}

QVariant QuCircularPlotI::itemChange(GraphicsItemChange change, const QVariant &value) {
    printf("\e[1;33m QuCircularPlotI.itemChange %d val %s\e[0m\n", change, qstoc(value.toString()));
    if(change == QGraphicsItem::ItemScaleChange ||
        change == QGraphicsItem::ItemVisibleChange) {
        pretty_pri("recalculating text font size");
        d->e->recalculateTxtFont();
    }
    return QGraphicsObject::itemChange(change, value);
}

void QuCircularPlotI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    printf("mouseReleaseEvent button %d\n", event->button());
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        update();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void QuCircularPlotI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}
