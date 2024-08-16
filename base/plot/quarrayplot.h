#ifndef QUARRAYPLOT_H
#define QUARRAYPLOT_H

#include <qwt_plot.h>
#include <vector>
#include <QPen>

class QuPlotP;
class QwtPlotCurve;
class QuCurves;

/*!
 * \brief Minimalistic extension of QwtPlot tailored for array data format (spectrum data)
 *
 * Refer to QuScalarPlot and
 * \ref quplotsng "the introduction to the scalar and array plots"
 * for general information, usage and rationale.
 *
 * Unlike QuScalarPlot, QuArrayPlot uses a QuArrayBuf rather than a QuCircularBuf.
 * Options, configuration, usage are pretty the same.
 *
 * See \ref quplotsng
 *
 * \subsection array_specific Array plot specific observations
 *
 * QuArrayPlot uses a linear buffer and setData uses the *move* semantics, meaning
 * data is no more valid after the call to setData. Alternatively, you must
 * fetch the curve and manually set data:
 *
 * \code
 void MyApp::onNewData(const std::vector<double> &y) {
    QuCurveAccess a(plot);  // a is local to the method, OK.
    QwtPlotCurve *c = a->get(name);
    QuArrayBuf *buf = static_cast<QuArrayBuf *>(c->data());
    buf->set(y);  // use set
    replot();
}
 * \endcode
 *
 * \par Example
 *
 * \code
    #include <quarrayplot.h>
    #include <quplotdataconnector.h>
    #include <cucontext.h>  // custom read period

    QuArrayPlot* plot = new QuArrayPlot(this);
    // OpenGL version
    // #include <quarrayplotgl.h>
    // QuArrayPlotGL *plot = new QuArrayPlotGL(this);
    plot->setObjectName("plot");
    QuPlotDataConnector *conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, plot);
    // optional: set up a periodic reading at 10Hz (#include <cucontext.h>)
    conn->getContext()->setOptions(CuData(TTT::Period, 100));
    // connect plot to data source(s) ($1 is a Tango device from the command line)
    conn->addSource("$1/fft");
 *  \endcode
 *
 */
class QuArrayPlot : public QwtPlot
{
    Q_OBJECT
public:
    QuArrayPlot(QWidget *parent, bool opengl = false);
    virtual ~QuArrayPlot();

    virtual QwtPlotCurve *addCurve(const std::string& name,
                           QwtAxisId xAxis = QwtAxis::XBottom,
                           QwtAxisId yAxis = QwtAxis::YLeft,
                                   const QPen& pen = QPen(QColor()));
    virtual void setData(const std::string &name, const std::vector<double>& y);
    virtual void onError(const std::string&name, const std::string& msg);

signals:
    void curveAdded(QwtPlotCurve *c);
    void dataUpdated(QwtPlotCurve *c);
    void error(const std::string& name, const std::string& msg);

private:

    QuPlotP *d;
};

#endif // QUARRAYPLOT_H
