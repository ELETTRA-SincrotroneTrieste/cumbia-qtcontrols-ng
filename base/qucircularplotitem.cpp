#include "qucircularplotengine.h"
#include "qucircularplotitem.h"
#include "quzoomer.h"

#include <cumacros.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

#include "quzoomer.h"
#include "quzoomevents.h"
#include "qucircularplotcurveselectionevents.h"
#include <QtDebug>

class QuCircularPlotI_P {
public:
    QuCircularPlotI_P(QGraphicsObject *gobj, const QFont& f, const QSize& siz)
        :  rect(0, 0, siz.width(), siz.height()) {
        QuCircularPlotCurveSelectionEvents *cse = new QuCircularPlotCurveSelectionEvents(gobj);
        QuZoomEvents *ze = new QuZoomEvents(gobj);
        e = new QuCircularPlotEngine(f, new QuZoomer(gobj), cse, ze);
        // circular plot engine takes the ownership of zoomer and zoom events
        // If multiple event filters are installed on a single object, the filter that was installed last is activated first.
        gobj->installEventFilter(ze); // 2nd
        gobj->installEventFilter(cse); // activated first 1st
    }
    ~QuCircularPlotI_P() {
        delete e;
    }
    QuCircularPlotEngine *e;
    QRectF rect;
};

QuCircularPlotI::QuCircularPlotI(QGraphicsItem *parent, const QSize &siz) : QGraphicsObject(parent) {
    d = new QuCircularPlotI_P(this, QFont(), siz);
    setAcceptHoverEvents(true); // mouse tracking
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    connect(d->e->zoomer(), SIGNAL(zoomChanged()), this, SLOT(update()));
    connect(d->e, SIGNAL(dirty()), this, SLOT(update()));
}

QuCircularPlotI::~QuCircularPlotI() {
    delete d;
}

void QuCircularPlotI::update() {
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

void QuCircularPlotI::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QGraphicsObject::contextMenuEvent(event);
}

void QuCircularPlotI::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mousePressEvent(event);
}

void QuCircularPlotI::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsObject::hoverMoveEvent(event);
}

void QuCircularPlotI::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsObject::hoverLeaveEvent(event);
}

QVariant QuCircularPlotI::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(change == QGraphicsItem::ItemScaleChange ||
        change == QGraphicsItem::ItemVisibleChange) {
        pretty_pri("recalculating text font size");
        d->e->recalculateTxtFont();
    }
    return QGraphicsObject::itemChange(change, value);
}

void QuCircularPlotI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mouseReleaseEvent(event);
}

void QuCircularPlotI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mouseDoubleClickEvent(event);
}
