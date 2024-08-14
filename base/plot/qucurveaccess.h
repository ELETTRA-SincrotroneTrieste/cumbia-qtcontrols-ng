#ifndef QUCURVEACCESS_H
#define QUCURVEACCESS_H

class QwtPlot;
class QwtPlotCurve;

#include <QList>
#include <string>

class QuCurveAccess
{
public:
    QuCurveAccess(QwtPlot *plot);
    QwtPlotCurve *get(const std::string& name) const;
    QList<QwtPlotCurve *> curves() const;

private:
    QwtPlot* m_p;
};

#endif // QUCURVEACCESS_H
