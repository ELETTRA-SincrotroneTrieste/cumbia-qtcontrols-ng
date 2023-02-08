#include "cuapplynumericengine.h"
#include "cuapplynumericitem.h"

#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>

class CuApplyNumericItemPrivate {
public:
    CuApplyNumericItemPrivate(const QFont& f) : e(new CuApplyNumericEngine(f)), rect(0, 0, 100, 40) {
    }
    ~CuApplyNumericItemPrivate() {
        delete e;
    }
    CuApplyNumericEngine *e;
    QRectF rect;
};

CuApplyNumericItem::CuApplyNumericItem(QGraphicsItem *parent) : QGraphicsObject(parent) {
    d = new CuApplyNumericItemPrivate(QFont());
}

CuApplyNumericItem::~CuApplyNumericItem() {
    delete d;
}


QRectF CuApplyNumericItem::boundingRect() const {
    return d->rect;
}

void CuApplyNumericItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    d->e->paint(painter, option->rect, widget);
}

void CuApplyNumericItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void CuApplyNumericItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void CuApplyNumericItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void CuApplyNumericItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void CuApplyNumericItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void CuApplyNumericItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
}
