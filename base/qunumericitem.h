#ifndef QUNUMERICITEM_H
#define QUNUMERICITEM_H

#include <QGraphicsObject>

class QuNumericI_P;

class QuNumericI : public QGraphicsObject
{
    Q_OBJECT
public:
    QuNumericI(QGraphicsItem *parent );
    virtual ~QuNumericI();

public slots:
    void update();

private:
    QuNumericI_P *d;

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

#endif // QUNUMERICITEM_H
