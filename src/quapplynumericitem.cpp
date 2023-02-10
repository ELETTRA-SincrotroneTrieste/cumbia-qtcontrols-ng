#include "quapplynumericclickrepeater.h"
#include "quapplynumericengine.h"
#include "quapplynumericitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class CuApplyNumericItemPrivate {
public:
    CuApplyNumericItemPrivate(const QFont& f) : e(new QuApplyNumericEngine(f)), rect(0, 0, 100, 40) {
    }
    ~CuApplyNumericItemPrivate() {
        delete e;
    }
    QuApplyNumericEngine *e;
    QRectF rect;
    CuApplyNumericClickRepeater click_r;
};

QuApplyNumericItem::QuApplyNumericItem(QGraphicsItem *parent) : QGraphicsObject(parent) {
    d = new CuApplyNumericItemPrivate(QFont());
    setAcceptHoverEvents(true); // mouse tracking

    connect(&d->click_r, SIGNAL(step(QPointF)), d->e, SLOT(click(QPointF)));
    connect(&d->click_r, SIGNAL(step(QPointF)), this, SLOT(update()));
}

QuApplyNumericItem::~QuApplyNumericItem() {
    delete d;
}

void QuApplyNumericItem::update() {
    QGraphicsObject::update(QRectF());
}


QRectF QuApplyNumericItem::boundingRect() const {
    const QSizeF& ms = d->e->minimumSize();
    d->rect.setRect(0, 0, ms.width() * 1.2, ms.height());
    return d->rect;
}

void QuApplyNumericItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    d->e->paint(painter, option->rect, widget);
}

void QuApplyNumericItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void QuApplyNumericItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        d->click_r.pressed(event->pos());
        update();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void QuApplyNumericItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void QuApplyNumericItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    d->e->mouseReleaseEvent(QPointF());
    printf("NumericItem: mouse release stop automation\n");
    d->click_r.released(QPointF());
    update();
}

void QuApplyNumericItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    printf("mouseReleaseEvent button %d\n", event->button());
    if(event->button() == Qt::LeftButton) {
        d->e->mouseReleaseEvent(d->click_r.active() ? QPointF() : event->pos());
        printf("NumericItem: mouse release stop automation\n");
        d->click_r.released(event->pos());
        update();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void QuApplyNumericItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void QuApplyNumericItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    d->e->wheelEvent(event->pos(), event->delta());
    update();
}
