#include "quplotzoomer.h"

#include "ScrollZoomer.h"
#include <qwt_plot.h>
#include <qwt_scale_widget.h>
#include <cumacros.h>
#include <QtDebug>

class QuPlotZoomerP {
public:
    QuPlotZoomerP() : plot(nullptr) {}
    bool autoscale_state[4]; // xb, yl, xtop, yright
    QwtPlot *plot;
};

QuPlotZoomer::QuPlotZoomer(QwtPlot *plot) : ScrollZoomer(plot->canvas()) {
    d = new QuPlotZoomerP;
    d->plot = plot;
}

QuPlotZoomer::~QuPlotZoomer() {
    delete d;
}

void QuPlotZoomer::zoom(const QRectF &r) {
    if(zoomRectIndex() == 0) {
        m_save_autoscale_state();
        setZoomBase();
    }
    ScrollZoomer::zoom(r);
}

void QuPlotZoomer::zoom( int offset ){
    ScrollZoomer::zoom(offset);
    if(zoomRectIndex() <= 0) {
        m_restore_autoscale_state();
        d->plot->replot();
    }
}

void QuPlotZoomer::m_save_autoscale_state() {
    for(int i = 0; i < QwtPlot::axisCnt; i++) {
        d->autoscale_state[i] = d->plot->axisAutoScale(i);
    }
}

void QuPlotZoomer::m_restore_autoscale_state() {
    for(int i = 0; i < QwtPlot::axisCnt; i++) {
        d->plot->setAxisAutoScale(i, d->autoscale_state[i]);
    }
}

