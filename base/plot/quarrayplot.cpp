#include "quarraybuf.h"
#include "quarrayplot.h"
#include "qucurves.h"
#include "quplotp.h"
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_engine.h>
#include <cumacros.h>


QuArrayPlot::QuArrayPlot(QWidget *parent, bool opengl) : QwtPlot(parent) {
    d = new QuPlotP(opengl, this);
    d->curves = new QuCurves();
    updateAxes();
}

QuArrayPlot::~QuArrayPlot() {
    delete d->curves;
    delete d;
}

QwtPlotCurve *QuArrayPlot::addCurve(const std::string &name,
                                    QwtPlot::Axis xAxis,
                                    QwtPlot::Axis yAxis,
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
    }
    emit curveAdded(c);
    return c;
}

/*!
 * \brief set Y data on the curve with the given *name*, using the *move* semantics on
 *        the internal data buffer.
 *
 * \param name the name of the curve you want data to be *moved* and displayed
 * \param y a const reference to a vector of double that will be *moved* to the
 *        internal storage.
 *
 * \par signals
 * emits dataUpdated with the involved QwtPlotCurve (with name *name*)
 *
 * If the curve with *name* is not found, addCurve is invoked to create one.
 */
void QuArrayPlot::setData(const std::string& name, const std::vector<double> &y) {
    QwtPlotCurve *c = d->curves->get(name);
    if(!c)
        c = addCurve(name);
    QuArrayBuf *buf = static_cast<QuArrayBuf *>(c->data());
    buf->move(y);
    replot();
    emit dataUpdated(c);
    if(toolTip() != QString())
        setToolTip(QString());
}

void QuArrayPlot::onError(const std::string &name, const std::string &msg) {
    emit error(name, msg);
    setToolTip(QString("%1:\n%2").arg(name.c_str(), msg.c_str()));
}


