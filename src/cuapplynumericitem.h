#ifndef CUAPPLYNUMERICITEM_H
#define CUAPPLYNUMERICITEM_H

#include <QGraphicsObject>

class CuApplyNumericItemPrivate;

class CuApplyNumericItem : public QGraphicsObject
{
    Q_OBJECT
public:
    CuApplyNumericItem(QGraphicsItem *parent );
    virtual ~CuApplyNumericItem();

private:
    CuApplyNumericItemPrivate *d;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // CUAPPLYNUMERICITEM_H
