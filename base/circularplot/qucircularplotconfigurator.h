#ifndef QUCIRCULARPLOTCONFIGURATOR_H
#define QUCIRCULARPLOTCONFIGURATOR_H

class QuCircularPlotCurve;

#include <QString>

class QuCircularPlotConfigurator
{
public:
    QuCircularPlotConfigurator();

    void configure(QuCircularPlotCurve *c, int curvecnt);

    QString m_get_id(const QuCircularPlotCurve *c);
};

#endif // QUCIRCULARPLOTCONFIGURATOR_H
