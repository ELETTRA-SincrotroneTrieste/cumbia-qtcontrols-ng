#include "quscalarplot.h"
#include "quplotp.h"
#include "qucurves.h"
#include "qucircularbuf.h"

#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <cumacros.h>
#include <qwt_scale_map.h>

class QuScalarPlotP {
public:
    QuScalarPlotP(size_t bufsize) :
        bufsiz(bufsize), dire_p(new QwtPlotDirectPainter) {}
    size_t bufsiz;
    QwtPlotDirectPainter *dire_p;
};

/*!
 * \brief class constructor with parent widget, buffer size and openGL option
 * \param p the parent widget
 * \param bufsiz the size of the circular buffer. Can be either a custom one or picked
 *        among QuCircularBuff::Duration
 * \param opengl
 */
QuScalarPlot::QuScalarPlot(QWidget *p, size_t bufsiz, bool opengl) : QwtPlot(p) {
    d = new QuPlotP(opengl, this);
    sd = new QuScalarPlotP(bufsiz);
    d->curves = new QuCurves();
}

QwtPlotCurve *QuScalarPlot::addCurve(const std::string &name,
                                     int xAxis,
                                     int yAxis,
                                     const QPen &pen) {
    QwtPlotCurve *c = nullptr;
    if(!d->curves->map.contains(name)) {
        // get a new curve with a default color if not specified
        c = d->curves->get(name, pen);
        c->attach(this);
        c->setAxes(xAxis, yAxis);
        bool xa = axisAutoScale(xAxis), ya = axisAutoScale(yAxis);
        QuCircularBuf* b = new QuCircularBuf(xa, ya, sd->bufsiz);
        c->setData(b);
        pretty_pri("addCurve on curve %p name '%s' with buf %p (data() returns buf %p)",
                   c, name.c_str(), b, static_cast<QuCircularBuf *>(c->data()));
    }
    emit curveAdded(c);
    return c;
}

void QuScalarPlot::append(const std::string &name, double x, double y)
{
    QwtPlotCurve *c = d->curves->get(name);
    if(!c)
        c = addCurve(name);
    QuCircularBuf *buf = static_cast<QuCircularBuf *>(c->data());
    bool bounds_changed = buf->append(&x, &y, 1);
    size_t siz = buf->size();
    size_t i2 = /*buf->index*/(siz - 1), i1 = /*buf->index*/(siz - 2);
    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        /*
                Depending on the platform setting a clip might be an important
                performance issue. F.e. for Qt Embedded this reduces the
                part of the backing store that has to be copied out - maybe
                to an unaccelerated frame buffer device.
             */

        const QwtScaleMap &xMap = canvasMap( c->xAxis() );
        const QwtScaleMap &yMap = canvasMap( c->yAxis() );
        const QRectF& br = qwtBoundingRect( *c->data(), i1, i2);
        const QRect& clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
        sd->dire_p->setClipRegion( clipRect );
    }

    if(bounds_changed) {
        replot();
    }
    else {
        printf("QuScalarPlot: \e[1;32mdrawing series from %ld to %ld\e[0m\n",
               i1, i2);
        sd->dire_p->drawSeries(c, i1, i2);
    }
    emit dataUpdated(c);
}

void QuScalarPlot::append(const std::string& name, double y) {
    QwtPlotCurve *c = d->curves->get(name);
    if(c) {
        QuCircularBuf *buf = static_cast<QuCircularBuf *>(c->data());
        buf->append(&y, 1);
        size_t siz = buf->size();
        size_t i2 = /*buf->index*/(siz - 1), i1 = /*buf->index*/(siz - 2);

        const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
        if ( doClip )
        {
            /*
                Depending on the platform setting a clip might be an important
                performance issue. F.e. for Qt Embedded this reduces the
                part of the backing store that has to be copied out - maybe
                to an unaccelerated frame buffer device.
             */

            const QwtScaleMap xMap = canvasMap( c->xAxis() );
            const QwtScaleMap yMap = canvasMap( c->yAxis() );
            QRectF br = qwtBoundingRect( *c->data(), i1, i2);
            const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
            sd->dire_p->setClipRegion( clipRect );
            printf("\e[1;31mCLIP RECT %d,%d, %dx%d\e[0m\n", clipRect.x(), clipRect.y(), clipRect.width(), clipRect.height());
        }
        else
            printf("\e[1;32m canvas has paint on screen attribute\e[0m\n");

        sd->dire_p->drawSeries(c, i1, i2);
        // replot();
        emit dataUpdated(c);
        if(toolTip() != QString())
            setToolTip(QString());
    }
}

void QuScalarPlot::onError(const std::string &name, const std::string &msg) {
    emit error(name, msg);
    setToolTip(QString("%1:\n%2").arg(name.c_str(), msg.c_str()));
}

