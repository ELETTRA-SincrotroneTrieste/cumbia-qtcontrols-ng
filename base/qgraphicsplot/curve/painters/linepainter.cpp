#include "linepainter.h"
#include "../scenecurve.h"
#include "../curveitem.h"
#include "../data.h"
#include "linepainterprivate.h"
#include <QtDebug>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPainter>
#include "qgraphicsplotmacros.h"

LinePainter::LinePainter(CurveItem *curveItem) : QObject(curveItem)
{
    d_ptr = new LinePainterPrivate();
    d_ptr->curveItem = curveItem;
    d_ptr->pen.setWidthF(0.0);
    d_ptr->brush = QBrush();
    curveItem->installItemPainterInterface(this);
    setObjectName("LinePainter");
}

LinePainter::~LinePainter()
{
    qDebug() << __FUNCTION__ << "deleting " << objectName() << "removing painter from curve item "
             << d_ptr->curveItem->objectName();
   // d_ptr->curveItem->removeItemPainterInterface(this);
    delete d_ptr;
}

void LinePainter::draw(SceneCurve *curve,
                  QPainter *painter,
                  const QStyleOptionGraphicsItem * option,
                  QWidget * w) {
    int dataSiz = curve->dataSize();
    painter->setClipRect(option->rect);
    painter->setPen(d_ptr->pen);
    const QPointF *points = curve->points();
    if(dataSiz <= 2) {
        painter->setBrush(QBrush(d_ptr->pen.color()));
        for(int i = 0; i < dataSiz; i++) {
            painter->drawEllipse(points[i], 3, 2.5);
        }
    }
    if(points && d_ptr->brush != QBrush() && dataSiz > 1) {
        if(painter->brush() != d_ptr->brush) {
            painter->setBrush(d_ptr->brush);
        }
        // calculate baseline coord
        double baseline = curve->baseline0().y();
        QPointF* polyp = new QPointF[2 + dataSiz];
        polyp[0] = QPointF(points[0].x(), baseline);
        memcpy(polyp + 1, points, sizeof(QPointF) * dataSiz);
        polyp[dataSiz + 1] = QPointF(points[dataSiz - 1].x(), baseline);
        painter->drawPolygon(polyp, dataSiz + 2, Qt::WindingFill);
        delete [] polyp;
        if(d_ptr->draw_baseline) {
            QPen p(d_ptr->brush.color().darker());
            painter->setPen(p);
            painter->drawLine(curve->baseline0(), curve->baseline1());
        }
    }
    else if(points) {
        painter->drawPolyline(points, dataSiz);
    }
    /* draw NaNs (invalid data */
    QVector<double> xInvalid = curve->data()->invalidDataPoints();
    if(xInvalid.size() > 0)
    {
        QPen invalidDataPen(Qt::red);
        invalidDataPen.setWidthF(0.0);
        painter->setPen(invalidDataPen);
        foreach(double d, xInvalid)
        {
            printf("%s \e[1;31mShould draw invalid points!!! \e[0m\n", __PRETTY_FUNCTION__);
//            painter->drawLine(plot->transform(d, plot->scaleItem(curve->getXAxis()->axisId())), 0,
//                              plot->transform(d, plot->scaleItem(curve->getXAxis()->axisId())),
//                              painter->clipBoundingRect().height());
        }
        painter->setPen(d_ptr->pen);
    }
}

int LinePainter::type() const
{
    return ItemPainterInterface::Line;
}

QSizeF LinePainter::elementSize() const
{
    return QSizeF(d_ptr->pen.widthF(), d_ptr->pen.widthF());
}

CurveItem *LinePainter::curveItem() const
{
    return d_ptr->curveItem;
}

QColor  LinePainter::lineColor() const
{
    return d_ptr->pen.color();
}

double LinePainter::lineWidth() const
{
    return d_ptr->pen.widthF();
}

QPen LinePainter::linePen() const
{
    return d_ptr->pen;
}

QPen LinePainter::pen() const
{
    return d_ptr->pen;
}

bool LinePainter::drawBaseline() const {
    return d_ptr->draw_baseline;
}

void LinePainter::setLineColor(const QColor& c)

{
    d_ptr->pen.setColor(c);
    d_ptr->curveItem->update();
}

void LinePainter::setLineWidth(double w)
{
    d_ptr->pen.setWidthF(w);
    d_ptr->curveItem->update();
}

void LinePainter::setLinePen(const QPen& p)
{
    d_ptr->pen = p;
    d_ptr->curveItem->update();
}

void LinePainter::setFillArea(const QBrush &b) {
    d_ptr->brush = b;
}

void LinePainter::setDrawBaseline(bool dra) {
    d_ptr->draw_baseline = dra;
}
