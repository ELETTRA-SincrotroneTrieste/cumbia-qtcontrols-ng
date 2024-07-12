#ifndef Arrayplot_H
#define Arrayplot_H

#include <QWidget>

// cumbia
#include <qulogimpl.h>
#include <cucontrolsfactorypool.h>
class CumbiaPool;
class CuData;
// cumbia

class QwtPlot;
class QwtPlotCurve;

namespace Ui {
class Arrayplot;
}

class Arrayplot : public QWidget
{
    Q_OBJECT

public:
    explicit Arrayplot(CumbiaPool *cu_p, QWidget *parent = 0);
    ~Arrayplot();

private slots:
    void autoscaleChanged(bool a);
    void scaleChanged();
    void srcConfigured(QwtPlot* p, const CuData &da);
    void curvesSelectionChanged();
    void moveCurveX();
    void moveCurveY();

private:
    Ui::Arrayplot *ui;

    void m_axes_visibility();

    // cumbia
    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuControlsFactoryPool m_ctrl_factory_pool;
    // cumbia
};

#endif // Arrayplot_H
