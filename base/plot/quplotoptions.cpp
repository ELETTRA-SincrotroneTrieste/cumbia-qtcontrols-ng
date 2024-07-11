#include "quplotoptions.h"
#include "qubufbase.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_div.h>

QuPlotOptions::QuPlotOptions(QwtPlot *plot) : m_plot(plot) {

}

void QuPlotOptions::xauto(bool a) {
    m_plot->setAxisAutoScale(QwtAxis::XBottom, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.xauto(a);
    }
}

void QuPlotOptions::yauto(bool a) {
    m_plot->setAxisAutoScale(QwtAxis::YLeft, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.yauto(a);
    }
}

void QuPlotOptions::x2auto(bool a) {
    m_plot->setAxisAutoScale(QwtAxis::XTop, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.xauto(a);
    }

}

void QuPlotOptions::y2auto(bool a)  {
    m_plot->setAxisAutoScale(QwtAxis::YRight, a);
    foreach(QwtPlotItem *c, m_plot->itemList(QwtPlotCurve::Rtti_PlotCurve)) {
        QuBufBase *bb = static_cast<QuBufBase *>(static_cast<QwtPlotCurve *>(c)->data());
        bb->o.yauto(a);
    }
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
}

void QuPlotOptions::xup(double up) {
    m_plot->setAxisScale(QwtPlot::xBottom, m_plot->axisScaleDiv(QwtPlot::xBottom).lowerBound(), up);
}

void QuPlotOptions::ylo(double lo) {
    m_plot->setAxisScale(QwtPlot::yRight, lo, m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound());
}

void QuPlotOptions::yup(double up) {
    m_plot->setAxisScale(QwtPlot::yRight, m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound(), up);
}

void QuPlotOptions::x2lo(double lo) {
    xauto(false);
    m_plot->setAxisScale(QwtPlot::xTop, lo, m_plot->axisScaleDiv(QwtPlot::xTop).upperBound());
}

void QuPlotOptions::x2up(double up) {
    m_plot->setAxisScale(QwtPlot::xTop, m_plot->axisScaleDiv(QwtPlot::xTop).lowerBound(), up);
}

void QuPlotOptions::y2lo(double lo) {
    m_plot->setAxisScale(QwtPlot::yLeft, lo, m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound());
}

void QuPlotOptions::y2up(double up) {
    m_plot->setAxisScale(QwtPlot::yLeft, m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound(), up);
}

bool QuPlotOptions::xlo() const {
    return m_plot->axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

bool QuPlotOptions::xup() const {
    return m_plot->axisScaleDiv(QwtPlot::xBottom).upperBound();
}

bool QuPlotOptions::ylo() const {
    return m_plot->axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

bool QuPlotOptions::yup() const {
    return m_plot->axisScaleDiv(QwtPlot::yLeft).upperBound();
}

bool QuPlotOptions::x2lo() const {
    return m_plot->axisScaleDiv(QwtPlot::xTop).lowerBound();
}

bool QuPlotOptions::x2up() const {
    return m_plot->axisScaleDiv(QwtPlot::xTop).upperBound();
}

bool QuPlotOptions::y2lo() const {
    return m_plot->axisScaleDiv(QwtPlot::yRight).lowerBound();
}

bool QuPlotOptions::y2up() const {
    return m_plot->axisScaleDiv(QwtPlot::yRight).upperBound();
}

