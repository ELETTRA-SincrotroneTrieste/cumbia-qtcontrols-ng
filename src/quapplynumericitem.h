#ifndef QUAPPLYNUMERICITEM_H
#define QUAPPLYNUMERICITEM_H

#include <QGraphicsObject>

class CuApplyNumericItemPrivate;

class QuApplyNumericItem : public QGraphicsObject
{
    Q_OBJECT
public:
    QuApplyNumericItem(QGraphicsItem *parent );
    virtual ~QuApplyNumericItem();

public slots:
    void update();

private:
    CuApplyNumericItemPrivate *d;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // QUAPPLYNUMERICITEM_H
