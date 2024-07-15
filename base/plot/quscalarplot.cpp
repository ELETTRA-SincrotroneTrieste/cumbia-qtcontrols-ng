#include "quscalarplot.h"
#include "quplotp.h"
#include "qucurves.h"
#include "qucircularbuf.h"

#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <cumacros.h>

/*!
 * \brief class constructor with parent widget, buffer size and openGL option
 * \param p the parent widget
 * \param bufsiz the size of the circular buffer. Can be picked from among QuCircularBuff::Duration
 * \param opengl
 */
QuScalarPlot::QuScalarPlot(QWidget *p, size_t bufsiz, bool opengl) : QwtPlot(p) {
    d = new QuPlotP(opengl, bufsiz);
    d->curves = new QuCurves();
    if(opengl) {
        setCanvas(d->make_GL_canvas() );
    } else {
        setCanvas(d->make_canvas());
    }
    d->align_scales(this);
    int axes[4]{ xBottom, xTop, yLeft, yRight};
    for(int axisId : axes )
        axisScaleEngine(axisId)->setAttribute(QwtScaleEngine::Floating);
    // Insert grid
    d->grid = new QwtPlotGrid();
    d->grid->attach( this );
}

QwtPlotCurve *QuScalarPlot::addCurve(const std::string &name,
                                     QwtAxisId xAxis,
                                     QwtAxisId yAxis,
                                     const QPen &pen) {
    QwtPlotCurve *c = nullptr;
    if(!d->curves->map.contains(name)) {
        // get a new curve with a default color if not specified
        c = d->curves->get(name, pen);
        c->attach(this);
        c->setAxes(xAxis, yAxis);
        bool xa = axisAutoScale(xAxis), ya = axisAutoScale(yAxis);
        QuCircularBuf* b = new QuCircularBuf(xa, ya, d->bufsiz);
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
    pretty_pri("%s -> value %f  y:[%.1f - %.1f] curve %p buf %p plot autoscale x %s, y %s",
               name.c_str(), y, buf->o.ylb, buf->o.yub, c, buf,
               axisAutoScale(QwtPlot::xBottom) ? "YES" : "NO",
               axisAutoScale(QwtPlot::yLeft) ? "YES": "NO");

    buf->append(&x, &y, 1);
    replot();
    emit dataUpdated(c);
}

void QuScalarPlot::append(const std::string& name, double y) {
    QwtPlotCurve *c = d->curves->get(name);
    if(c) {
        QuCircularBuf *buf = static_cast<QuCircularBuf *>(c->data());
        // pretty_pri("%s -> data siz %ld: y:[%.1f - %.1f] curve %p buf %p plot autoscale x %s, y %s",
        //            name.c_str(), y.size(), buf->o.ylb, buf->o.yub, c, buf,
        //            axisAutoScale(QwtPlot::xBottom) ? "YES" : "NO",
        //            axisAutoScale(QwtPlot::yLeft) ? "YES": "NO");

        buf->append(&y, 1);
        replot();
        emit dataUpdated(c);
    }
}

void QuScalarPlot::onError(const std::string &name, const std::string &msg) {
    emit error(name, msg);
}
