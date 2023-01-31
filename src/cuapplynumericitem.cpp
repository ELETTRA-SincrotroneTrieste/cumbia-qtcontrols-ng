#include "cuapplynumericengine.h"
#include "cuapplynumericitem.h"

class CuApplyNumericItemPrivate {
public:
    CuApplyNumericEngine e;
};

CuApplyNumericItem::CuApplyNumericItem() {
    d = new CuApplyNumericItemPrivate;
}

CuApplyNumericItem::~CuApplyNumericItem() {
    delete d;
}


QRectF CuApplyNumericItem::boundingRect() const {
}

void CuApplyNumericItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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
