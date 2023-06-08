#include "qunumericclickrepeater.h"
#include "qunumericengine.h"
#include "qunumericitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class QuNumericI_P {
public:
    QuNumericI_P(const QFont& f) : e(new QuNumericEngine(f)), rect(0, 0, 100, 40) {
    }
    ~QuNumericI_P() {
        delete e;
    }
    QuNumericEngine *e;
    QRectF rect;
    QuNumericClickRepeat click_r;
};

QuNumericI::QuNumericI(QGraphicsItem *parent) : QGraphicsObject(parent) {
    d = new QuNumericI_P(QFont());
    setAcceptHoverEvents(true); // mouse tracking

    connect(&d->click_r, SIGNAL(step(QPointF)), d->e, SLOT(click(QPointF)));
    connect(&d->click_r, SIGNAL(step(QPointF)), this, SLOT(update()));
}

QuNumericI::~QuNumericI() {
    delete d;
}

void QuNumericI::update() {
    QGraphicsObject::update(QRectF());
}


QRectF QuNumericI::boundingRect() const {
    const QSizeF& ms = d->e->minimumSize();
    d->rect.setRect(0, 0, ms.width() * 1.2, ms.height());
    return d->rect;
}

void QuNumericI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    d->e->paint(painter, option->rect, widget);
}

void QuNumericI::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void QuNumericI::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        d->click_r.pressed(event->pos());
        update();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void QuNumericI::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void QuNumericI::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    d->e->mouseReleaseEvent(QPointF());
    printf("NumericItem: mouse release stop automation\n");
    d->click_r.released(QPointF());
    update();
}

void QuNumericI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
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

void QuNumericI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void QuNumericI::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    d->e->wheelEvent(event->pos(), event->delta());
    update();
}
