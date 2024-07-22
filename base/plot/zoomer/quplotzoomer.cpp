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
    // connect(plot->axisWidget(QwtAxis::XBottom), SIGNAL(scaleDivChanged ()),
    //         this, SLOT(onAxisScaleDivChanged()));
}

QuPlotZoomer::~QuPlotZoomer() {
    delete d;
}

void QuPlotZoomer::onAxisScaleDivChanged() {
    QwtScaleWidget *s = qobject_cast<QwtScaleWidget *>(sender());
    printf("QuPlotZoomer.onAxisScaleDivChanged: \e[1;35m%s\e[0m scale changed ", s->alignment() == QwtScaleDraw::BottomScale ? "x" : "y");
    QwtPlot::Axis a;
    if(s->alignment() == QwtScaleDraw::BottomScale) a = QwtPlot::xBottom;
    else if(s->alignment() == QwtScaleDraw::TopScale) a = QwtPlot::xTop;
    else if (s->alignment() == QwtScaleDraw::LeftScale) a = QwtPlot::yLeft;
    else a = QwtPlot::yRight;
    printf("  [ %f, %f ]\n", d->plot->axisScaleDiv(a).lowerBound(),
        d->plot->axisScaleDiv(a).upperBound());
}

void QuPlotZoomer::zoom(const QRectF &r) {
    qDebug() << __PRETTY_FUNCTION__ << "zoom with rect " << r;
    if(zoomRectIndex() == 0) {
        m_save_autoscale_state();
        // setZoomBase();
    }
    // ScrollZoomer::zoom(r);
}

void QuPlotZoomer::zoom( int offset ){
    qDebug() << __PRETTY_FUNCTION__ << "zoom with offset " << offset
             << "prev index" << zoomRectIndex() << "current offset " << offset;

    ScrollZoomer::zoom(offset);
    if(zoomRectIndex() <= 0) {
        printf("\e[1;32mUNZOOMED TO ZOOM BASE!\e[0m");
        m_restore_autoscale_state();
    }
    printf("after zoom, rect index is %d\n", zoomRectIndex());

}

void QuPlotZoomer::m_save_autoscale_state() {
    for(int i = 0; i < QwtPlot::axisCnt; i++) {
        d->autoscale_state[i] = d->plot->axisAutoScale(i);
        pretty_pri("%d: %s", i, d->plot->axisAutoScale(i) ? "AUTO" : "MANUAL");
    }
}

void QuPlotZoomer::m_restore_autoscale_state() {
    for(int i = 0; i < QwtPlot::axisCnt; i++) {
        d->plot->setAxisAutoScale(i, d->autoscale_state[i]);
        pretty_pri("%d: %s", i, d->plot->axisAutoScale(i) ? "AUTO" : "MANUAL");

    }
}

