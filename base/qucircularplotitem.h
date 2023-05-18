#ifndef QUCIRCULARPLOTITEM_H
#define QUCIRCULARPLOTITEM_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
class QuCircularPlotI_P;
class QuCircularPlotEngine;

class QuCircularPlotI : public QGraphicsObject
{
    Q_OBJECT
public:
    QuCircularPlotI(QGraphicsItem *parent );
    virtual ~QuCircularPlotI();

public slots:
    void update();
    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);
    QuCircularPlotEngine *engine() const;

private:
    QuCircularPlotI_P *d;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // QUCIRCULARPLOTITEM_H
