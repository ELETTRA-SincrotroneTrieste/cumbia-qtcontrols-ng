#ifndef QUPLOTZOOMER_H
#define QUPLOTZOOMER_H

#include <QwtPlot>
#include "ScrollZoomer.h"

class QuPlotZoomerP;

class QuPlotZoomer : public ScrollZoomer
{
    Q_OBJECT
public:
    QuPlotZoomer(QwtPlot *plot);
    virtual ~QuPlotZoomer();

public slots:
    virtual void zoom( const QRectF& );
    void zoom(int offset);

private:
    void m_save_autoscale_state();
    void m_restore_autoscale_state();
    QuPlotZoomerP *d;
};

#endif // QUPLOTZOOMER_H
