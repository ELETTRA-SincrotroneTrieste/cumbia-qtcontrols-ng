#include "qucircularplotengine.h"
#include "qucircularplotitem.h"
#include "quzoomer.h"

#include <cumacros.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class QuCircularPlotI_P {
public:
    QuCircularPlotI_P(QGraphicsObject *gobj, const QFont& f)
        : e(new QuCircularPlotEngine(f, new QuZoomer(gobj))), rect(0, 0, 100, 40) {
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
    printf("\e[1;33m change %d val %s\e[0m\n", change, qstoc(value.toString()));
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
