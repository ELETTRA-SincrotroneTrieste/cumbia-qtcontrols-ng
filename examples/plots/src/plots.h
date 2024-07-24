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

class Plots : public QWidget
{
    Q_OBJECT

public:
    explicit Plots(CumbiaPool *cu_p, QWidget *parent = 0);
    ~Plots();

private slots:
    void autoscaleChanged(bool a);
    void scaleChanged();
    void initControls(QwtPlot* p);
    void curvesSelectionChanged();
    void moveCurveX();
    void moveCurveY();
    void updateScalarPlot();
    QTimer *m_datasim_setup(const QString &plotnam, int period);

private:
    Ui::Arrayplot *ui;
    bool array;

    QwtPlot *m_plot(const QString &plotn) const;

    void m_axes_visibility();

    // cumbia
    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuControlsFactoryPool m_ctrl_factory_pool;
    // cumbia
};

#endif // Arrayplot_H
