#include "quplotoptions.h"
#include "qubufbase.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_div.h>
#include <cumacros.h>
#include <qwt_plot_opengl_canvas.h>
#include <qwt_plot_canvas.h>

QuPlotOptions::QuPlotOptions(QwtPlot *plot) : m_plot(plot) {

}

void QuPlotOptions::xauto(bool a) {
    m_plot->setAxisAutoScale(QwtPlot::xBottom, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.xauto(a);
    }
    m_plot->replot();
}

void QuPlotOptions::yauto(bool a) {
    m_plot->setAxisAutoScale(QwtPlot::yLeft, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.yauto(a);
    }
    m_plot->replot();
}

void QuPlotOptions::x2auto(bool a) {
    m_plot->setAxisAutoScale(QwtPlot::xTop, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.xauto(a);
    }
    m_plot->replot();

}

void QuPlotOptions::y2auto(bool a)  {
    m_plot->setAxisAutoScale(QwtPlot::yRight, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.yauto(a);
    }
    m_plot->replot();
}

bool QuPlotOptions::xauto() const {
    return m_plot->axisAutoScale(QwtPlot::xBottom);
}

bool QuPlotOptions::yauto() const {
    return m_plot->axisAutoScale(QwtPlot::yLeft);
}

bool QuPlotOptions::x2auto() const {
    return m_plot->axisAutoScale(QwtPlot::xTop);
}

bool QuPlotOptions::y2auto() const {
    return m_plot->axisAutoScale(QwtPlot::yRight);
}

void QuPlotOptions::xlo(double lo) {
    xauto(false);
    m_plot->setAxisScale(QwtPlot::xBottom, lo, m_plot->axisScaleDiv(QwtPlot::xBottom).upperBound());
    m_plot->replot();
}

void QuPlotOptions::xup(double up) {
    m_plot->setAxisScale(QwtPlot::xBottom, m_plot->axisScaleDiv(QwtPlot::xBottom).lowerBound(), up);
    m_plot->replot();
}

void QuPlotOptions::ylo(double lo) {
    m_plot->setAxisScale(QwtPlot::yLeft, lo, m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound());
    m_plot->replot();
}

void QuPlotOptions::yup(double up) {
    m_plot->setAxisScale(QwtPlot::yLeft, m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound(), up);
    m_plot->replot();
    pretty_pri("plot bounds Y [%f, %f]", m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound(),
               m_plot->axisScaleDiv(QwtPlot::yLeft).upperBound());
}

void QuPlotOptions::x2lo(double lo) {
    xauto(false);
    m_plot->setAxisScale(QwtPlot::xTop, lo, m_plot->axisScaleDiv(QwtPlot::xTop).upperBound());
    m_plot->replot();
}

void QuPlotOptions::x2up(double up) {
    m_plot->setAxisScale(QwtPlot::xTop, m_plot->axisScaleDiv(QwtPlot::xTop).lowerBound(), up);
    m_plot->replot();
}

void QuPlotOptions::y2lo(double lo) {
    m_plot->setAxisScale(QwtPlot::yRight, lo, m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound());
    m_plot->replot();
}

void QuPlotOptions::y2up(double up) {
    m_plot->setAxisScale(QwtPlot::yRight, m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound(), up);
    m_plot->replot();
}

void QuPlotOptions::opengl(bool ogl) {
    if(ogl) {
        QwtPlotOpenGLCanvas* canvas = new QwtPlotOpenGLCanvas();
        canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
        canvas->setLineWidth( 1 );
        canvas->setPalette( Qt::white );
        m_plot->setCanvas(canvas);
    }
    else {
        QwtPlotCanvas* canvas = new QwtPlotCanvas();
        canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
        canvas->setLineWidth( 1 );
        canvas->setPalette(Qt::white);
        m_plot->setCanvas(canvas);
    }
}

double QuPlotOptions::xlo() const {
    return m_plot->axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

double QuPlotOptions::xup() const {
    return m_plot->axisScaleDiv(QwtPlot::xBottom).upperBound();
}

double QuPlotOptions::ylo() const {
    return m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

double QuPlotOptions::yup() const {
    return m_plot->axisScaleDiv(QwtPlot::yLeft).upperBound();
}

double QuPlotOptions::x2lo() const {
    return m_plot->axisScaleDiv(QwtPlot::xTop).lowerBound();
}

double QuPlotOptions::x2up() const {
    return m_plot->axisScaleDiv(QwtPlot::xTop).upperBound();
}

double QuPlotOptions::y2lo() const {
    return m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound();
}

double QuPlotOptions::y2up() const {
    return m_plot->axisScaleDiv(QwtPlot::yRight).upperBound();
}

bool QuPlotOptions::opengl() const {
    return qobject_cast<QwtPlotOpenGLCanvas *>(m_plot->canvas());
}

