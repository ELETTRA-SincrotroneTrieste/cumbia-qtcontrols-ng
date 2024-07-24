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
 *  The interface of *cumbia-qtcontrols-ng* plots is minimal to ensure maximum performance
 *  of the basic objects.
 *
 *  Additional functionality is added by installing additional decorators / observers.
 *
 *  Data is stored in a *circular buffer*, namely QuCircularBuf, whose size shall be specified
 *  in the constructor. If unspecified, the value QuCircularBuf::Day_Secs is used by default,
 *  providing enough space for one day of data at one second rate.
 *
 */
class QuScalarPlot : public QwtPlot
{
    Q_OBJECT
public:
    QuScalarPlot(QWidget *p, size_t bufsiz = QuCircularBuf::Day_Secs, bool opengl = false);

    QwtPlotCurve *addCurve(const std::string &name,
                           QwtAxisId xAxis = QwtAxis::XBottom,
                           QwtAxisId yAxis = QwtAxis::YLeft,
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
