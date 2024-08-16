#ifndef QUCURVEACCESS_H
#define QUCURVEACCESS_H

class QwtPlot;
class QwtPlotCurve;

#include <QList>
#include <string>

/*!
 * \brief access the curves of a given plot
 *
 * Get the list of curves or a curve by name
 *
 * The class can be simply used locally:
 *
 * \code
 * void MyPlotApp::f() {
 *      QuArrayPlot *plot = findChild<QuArrayPlot *>();
 *      QuCurveAccess a(plot);
 *      QwtPlotCurve *t = a.get("temperature");
 *      // use t
 *  }
 * \endcode
 */
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
