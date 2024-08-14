#include "qucurveaccess.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>

QuCurveAccess::QuCurveAccess(QwtPlot *plot) : m_p(plot) {}

QwtPlotCurve *QuCurveAccess::get(const std::string &name) const {
    const QwtPlotItemList& itl = m_p->itemList();
    for ( QwtPlotItemIterator it = itl.begin(); it != itl.end(); ++it )
        if ( ( *it )->rtti() == QwtPlotItem::Rtti_PlotCurve && static_cast<QwtPlotCurve *>(*it)->title().text().toStdString() == name )
            return static_cast<QwtPlotCurve *>(*it);
    return nullptr;
}

QList<QwtPlotCurve *> QuCurveAccess::curves() const
{
    QList<QwtPlotCurve *> l;
    const QwtPlotItemList& itmList = m_p->itemList();
    for ( QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it )
        if ( ( *it )->rtti() == QwtPlotItem::Rtti_PlotCurve )
            l.push_back(static_cast<QwtPlotCurve *>(*it));
    return l;
}
