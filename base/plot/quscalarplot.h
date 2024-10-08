#ifndef QUSCALARPLOT_H
#define QUSCALARPLOT_H

#include <QPen>
#include <qwt_plot.h>
#include <qucircularbuf.h>

class QuPlotP;
class QuScalarPlotP;
class QwtPlotCurve;
class QuCurves;

/*! Plot specialized in the representation of curves with scalar data added over time
 *
 *  See \ref quplotsng "the introduction to the scalar and array plots"
 *
 *  The implementation of *cumbia-qtcontrols-ng* plots has been kept minimal to ensure
 *  maximum performance and zero code potentially affecting speed.
 *
 *  For this reason, extra functionality is added by installing additional objects,
 *  such as QuPlotZoomer, QuCurveSelector, QuAxisEventFilter.
 *

 *
 *  \par Connecting the plot to sources of data
 *  The QuPlotDataConnector helper class can be used to connect a plot to a source
 *  of (scalar) data. QuPlotDataConnector is a *cumbia* aware object that easily
 *  updates curves on plots.
 *
 *  \subsection scalar_specific Scalar plot specific observations
 *
 *  \subsubsection circular_buf Circular buffer
 *  Data is stored in a *circular buffer*, namely QuCircularBuf, whose size shall be specified
 *  in the constructor. If unspecified, the value QuCircularBuf::Day_Secs is used by default,
 *  providing enough space for one day of data at 1 second rate.
 *
 *  \subsubsection x_axis_time X axis time scale
 *  QuXTimeScale is typically installed on QuScalarPlot in order to provide a time
 *  (and optionally the date) representation of data on the X-axis.
 *
 *  \subsubsection append_methods append methods
 *  Scalar data is *added* point by point by one of the two available append
 *  methods.
 *
 *  \code
    #include <quscalarplot.h>
    #include <quplotdataconnector.h>
    #include <cucontext.h>
    #include <quxtimescale.h>

    QuScalarPlot* plot = new QuScalarPlot(this);
    plot->setObjectName("plot");
    QuPlotDataConnector *conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, plot);
    // optional: set up a periodic reading at 2Hz (#include <cucontext.h>)
    conn->getContext()->setOptions(CuData(TTT::Period, 500));
    // configure both x axes (top and bottom) with a time scale and label rotation
    int x_axes[2] { QwtPlot::xBottom, QwtPlot::xTop};
    for(int i = 0; i < 2; i++)
        new QuXTimeScale(plot, x_axes[i]);
    // connect plot to data source(s)
    conn->addSource("$1/temperature");
    conn->addSource("$2/temperature");
 *  \endcode
 *
 */
class QuScalarPlot : public QwtPlot
{
    Q_OBJECT
public:
    QuScalarPlot(QWidget *p, size_t bufsiz = QuCircularBuf::Day_Secs, bool opengl = false);

    QwtPlotCurve *addCurve(const std::string &name,
                           int xAxis = QwtPlot::xBottom,
                           int yAxis = QwtPlot::yLeft,
                           const QPen& pen = QPen(QColor()));
    void append(const std::string &name, double x, double y);
    void append(const std::string &name, double y);
    void onError(const std::string &name, const std::string &msg);

signals:
    void curveAdded(QwtPlotCurve *c);
    void dataUpdated(QwtPlotCurve *c);
    void error(const std::string& name, const std::string& msg);

private:
    QuPlotP *d;
    QuScalarPlotP *sd;
};

#endif // QUSCALARPLOT_H
