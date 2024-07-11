#include "qucurves.h"
#include <qupalette.h>
#include <QColor>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>

QuCurves::QuCurves() {
    pens = QVector<QPen> { QPen(QBrush("Blue"), 0),
                         QPen(QBrush("ForestGreen"), 0),
                         QPen(QBrush("Orange"), 0),
                         QPen(QBrush("Magenta"), 0),
                         QPen(QBrush("MediumTurquoise"), 0),
                         QPen(QBrush("OrangeRed"), 0),
                         QPen(QBrush("LightBlue"), 0),
                         QPen(QBrush("DimGray"), 0),
                         QPen(QBrush("Gold"), 0),
                         QPen(QBrush("LightSteelBlue"), 0),
                         QPen(QBrush("PaleGreen"), 0),
                         };
}

/*! add a new curve or return the curve with the given name if present
 */
QwtPlotCurve *QuCurves::get(const std::string &name, const QPen &p) {
    QwtPlotCurve *c = map.value(name);
    if(c)
        return c;
    c = new QwtPlotCurve(name.c_str());
    map[name] = c;
    p.color().isValid() ? c->setPen(p) : c->setPen(pens.at(pens.size() % map.size()));
    return c;
}
