#include "quarraybuf.h"
#include "quarrayplot.h"
#include "qucurves.h"
#include "quplotp.h"
#include <qwt_plot_opengl_canvas.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_engine.h>
#include <cumacros.h>


QuArrayPlot::QuArrayPlot(QWidget *parent, bool opengl) : QwtPlot(parent) {
    d = new QuPlotP(opengl);
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

QuArrayPlot::~QuArrayPlot() {
    delete d->curves;
    delete d;
}

QwtPlotCurve *QuArrayPlot::addCurve(const std::string &name,
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
        QuArrayBuf* b = new QuArrayBuf(xa, ya);
        c->setData(b);
        pretty_pri("addCurve on curve %p with buf %p (data() returns buf %p)",
                   c, b, static_cast<QuArrayBuf *>(c->data()));
    }
    emit curveAdded(c);
    return c;
}

void QuArrayPlot::setData(const std::string& name, const std::vector<double> &y) {
    QwtPlotCurve *c = d->curves->get(name);
    if(!c)
        c = addCurve(name);
    QuArrayBuf *buf = static_cast<QuArrayBuf *>(c->data());
    buf->move(y);
    replot();
    emit dataUpdated(c);

}

void QuArrayPlot::onError(const std::string &name, const std::string &msg) {
    emit error(name, msg);
}

