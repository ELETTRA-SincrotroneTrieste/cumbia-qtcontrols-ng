#ifndef QUARRAYPLOT_H
#define QUARRAYPLOT_H

#include <qwt_plot.h>
#include <vector>
#include <QPen>

class QuPlotP;
class QwtPlotCurve;
class QuCurves;

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
