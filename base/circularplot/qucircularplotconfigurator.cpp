#include "qucircularplotconfigurator.h"
#include <QApplication>
#include <QSettings>
#include <QString>
#include <QPen>
#include <qucircularplotcurve.h>
#include <qupalette.h>
#include <cumacros.h>

QuCircularPlotConfigurator::QuCircularPlotConfigurator()
{

}

void QuCircularPlotConfigurator::configure(QuCircularPlotCurve *c, int curves_cnt)
{
    QSettings s("elettra.eu", qApp->applicationName());
    const QString& id = m_get_id(c);
    QStringList groups = s.childGroups();
    int idx = groups.indexOf(id);
    if(c != nullptr && idx >= 0) {
        s.beginGroup(id);
        printf("\e[1,33m loading from settings %s\e[0m\n", qstoc(id));
        /* choose a nice curve color or load it from QSettings, if defined for the application name and
         * curve name.
         */
        QColor curveColor = s.value("pen/color", c->pen().color()).value<QColor>();
        int alpha = s.value("pen/color/alpha", c->pen().color().alpha()).toInt();
//        int curveStyle = s.value("pen/style", c->style()).toInt();
        double penWidth = s.value("pen/width", c->pen().widthF()).toDouble();
        //        bool displayYValues = s.value(id + "/show-values", false).toBool();
        curveColor.setAlpha(alpha);
        QPen curvePen(curveColor);
        curvePen.setWidth(penWidth);
        pretty_pri("pen width %f", penWidth);
        c->setPen(curvePen);
        //        c->setProperty("showYValuesEnabled", displayYValues);
        s.endGroup();
    }
    else if(c != nullptr) {
        QuPalette palette;
        QStringList colors = QStringList() << "dark_green" << "blue" << "violet"
                                           << "red" << "black" << "light_gray" << "yellow" <<  "green" << "gray"
                                           << "orange" << "pink" << "dark_red";
        QString color_nam = colors.at(curves_cnt % colors.size());
        QColor curveColor = palette.value(color_nam);
        QPen curvePen(curveColor, 0.0);
        c->setPen(curvePen);
    }
}

QString QuCircularPlotConfigurator::m_get_id(const QuCircularPlotCurve *c) {
    return c->source().replace("/", ".");
}
