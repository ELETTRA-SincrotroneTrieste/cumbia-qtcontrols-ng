#ifndef QUCIRCULARPLOTITEM_H
#define QUCIRCULARPLOTITEM_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
class QuCircularPlotI_P;
class QuCircularPlotEngine;

/*!
 * \brief A QGraphicsItem (QGraphicsObject) representing a circular plot
 *
 * QuCircularPlotI is a QGraphicsItem (actually, a QGraphicsObject) draws
 * curves on a circle, between a lower and an upper bound circumference
 *
 * \see QuCircularPlotEngine
 * \see QuCircularPlotWidget
 *
 * Refer to the QuCircularPlotEngine for further information and an example.
 *
 * \code
   QuCircularPlotI *ni = new QuCircularPlotI(nullptr, QSize(300, 300));
   QuCircularPlotEngine *pie = ni->engine();
   // now use QuCircularPlotEngine interface to configure the plot
 * \endcode
 *
 *
 * QuCircularPlotW and QuCircularPlotItem share the same painting engine to draw their contents
 * using the classical QWidget and the QGraphicsView/QGraphicsScene technologies respectively.
 *
 * \image html circularplots.png "Four QuCircularPlotI items in a QGraphicsScene. One is zoomed."
 * \image latex circularplots.png "Four QuCircularPlotI items in a QGraphicsScene. One is zoomed." width=10cm
 *
 */
class QuCircularPlotI : public QGraphicsObject
{
    Q_OBJECT
public:
    QuCircularPlotI(QGraphicsItem *parent, const QSize& siz = QSize(200,200));
    virtual ~QuCircularPlotI();
    QuCircularPlotEngine *engine() const;

public slots:
    void update(); // we need a slot: there's no QGraphicsObject::update slot
    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);

private:
    QuCircularPlotI_P *d;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // QUCIRCULARPLOTITEM_H
