#ifndef QUCURVES_H
#define QUCURVES_H

#include <QMap>
#include <QPen>
#include <QVector>
#include <string>

class QwtPlotCurve;
class QwtPlot;

class QuCurves
{
public:
    QuCurves();

    QwtPlotCurve *get(const std::string& name, const QPen& p = QPen());

    QMap<std::string, QwtPlotCurve *> map;
    QVector<QPen> pens;
};

#endif // QUCURVES_H
