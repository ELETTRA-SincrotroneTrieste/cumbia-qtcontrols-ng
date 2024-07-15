#ifndef QUSCALARPLOT_H
#define QUSCALARPLOT_H

#include <QPen>
#include <qwt_plot.h>
#include <qucircularbuf.h>

class QuPlotP;
class QwtPlotCurve;

class QuScalarPlot : public QwtPlot
{
    Q_OBJECT
public:
    QuScalarPlot(QWidget *p, size_t bufsiz, bool opengl = false);

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
};

#endif // QUSCALARPLOT_H
