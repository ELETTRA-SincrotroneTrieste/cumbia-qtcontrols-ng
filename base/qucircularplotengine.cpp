#include "qucircularplotengine.h"
#include "out_of_bounds_xforms.h"
#include <math.h>
#include <QPainter>
#include <QtDebug>
#include <QWidget>
#include <cumacros.h>
#include <QTimer>
#include <qpainter.h>
#include <qrect.h>
#include <math.h>
#include <qucircularplotconfigurator.h>

#define CHECK_MARK "✓"


QuCircularPlotEngineData::QuCircularPlotEngineData()
    : bounding_r(0, 0, 100, 100), a0(0), a1(360.0),
    xlb(0.0), xub(0.0), ylb(0.0), yub(0.0),
    xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), baseline(0.0),
    x_autoscale(true), y_autoscale(true),
    show_values(false), show_points(false), show_bounds(true),
    show_curves(true),
    show_xax(true), show_origin(true) , y_axes(0)
{
    radius = 100 / 8.0;  // side / 2 / 4
    radius_factor = 0.5;
    inner_radius_factor = 0.2;
    outer_radius_factor = 0.8;
    maxdatasiz = 0;
    oob_xform = new OOBLogTransform;
}

QuCircularPlotEngine::QuCircularPlotEngine(const QFont& f, QuZoomer *zoomer)
    : QObject{nullptr}  {
    d.font = f;
    d.zoomer = zoomer;
}

QuCircularPlotEngine::~QuCircularPlotEngine() {

}

void QuCircularPlotEngine::setData(const QString &src, const QVector<double> &xdata, const QVector<double> &ydata){
    QuCircularPlotConfigurator pco;
    if(!d.curves.contains(src)) {
        d.curves[src] = new QuCircularPlotCurve(src);
        pco.configure(d.curves[src], d.curves.size());
    }
    d.curves[src]->setData(xdata, ydata);
    m_get_bounds_from_curves(&d.xmin, &d.xmax, &d.ymin, &d.ymax, &d.maxdatasiz);
    if(d.x_autoscale) {
        d.xlb = d.xmin; d.xub = d.xmax;
    }
    if(d.y_autoscale) {
        d.ylb = d.ymin; d.yub = d.ymax;
    }
}

void QuCircularPlotEngine::m_get_bounds_from_curves(double *x, double *X, double *y, double *Y, int *max_datasiz) const
{
    int i = 0;
    *max_datasiz = 0;
    foreach(const QuCircularPlotCurve* c, d.curves) {
        const QVector<double> &xd = c->x_data();
        if(*max_datasiz < c->size())
            *max_datasiz = c->size();
        if(i++ == 0 && c->size() > 0) {
            *x = xd.first();
            *X = xd.last();
            *y = c->ymin();
            *Y = c->ymax();
        } else if(c->size() > 0) {
            if(*x > xd.first())
                *x = xd.first();
            if(*X < xd.last())
                *X = xd.last();
            if(*y > c->ymin())
                *y = c->ymin();
            if(*Y < c->ymax())
                *Y = c->ymax();
        }
    }
}

void QuCircularPlotEngine::m_get_bounds(double *x, double *X,
                                        double *y, double *Y,
                                        double *inr, double *outr,
                                        const double &R) const
{
    d.oob_xform->bounds_transform(x, X, y, Y, d.xlb, d.xub, d.ylb, d.yub, d.xmin, d.xmax, d.ymin, d.ymax);

    // if curve values fall beyond our axis span, use all the available space
    *inr = /*d.ylb > d.ymin ? d.inner_radius_factor / 2.0 * R :*/ R * d.inner_radius_factor;
    *outr = /*d.yub < d.ymax ? 0.98 * R :*/  R * d.outer_radius_factor;
}

void QuCircularPlotEngine::paint(QPainter *p, const QRectF &rect, QWidget *widget) {
    if(d.zoomer)
        printf("zoomer in zoom? %s level %d\n", d.zoomer->inZoom() ? "YES" : "NO", d.zoomer->stackSize());
    if(d.bounding_r != rect) {
        d.bounding_r = rect;
    }
    p->setRenderHint(QPainter::Antialiasing, true);
    double R = qMin(rect.width(), rect.height()) / 2.0;
    const QPointF& c = rect.center();
    d.radius = R * d.radius_factor;
    // draw curves

    if(d.show_values || d.show_bounds) {
        p->setFont(d.scaled_font);
    }
    QPen txtpen(Qt::gray, 0.0);

    // if manual bounds are not wide enough, we must make corrections to draw
    // the most accurately possible
    double xmin, xmax, ymin, ymax, inr, outr;
    m_get_bounds(&xmin, &xmax, &ymin, &ymax, &inr, &outr, R);
    foreach(const QuCircularPlotCurve* c, d.curves) {
        QPointF *points = this->m_get_points(c, R, xmin, xmax, ymin, ymax, inr, outr);
        p->setPen(c->pen());
        if(d.show_curves)
            p->drawPolyline(points, c->size());
        if(d.show_points)
            p->drawPoints(points, c->size());
        if(d.show_values) {
            p->setPen(txtpen);
            for(int i = 0; i < c->size(); i++)
                p->drawText(points[i], QString::number(c->y_data()[i]));
        }
        delete [] points;
    }

    QPen pe(Qt::lightGray, 0.0);
    p->setPen(pe);

    if(d.baseline >= d.ylb && d.baseline <= d.yub) {
        // draw circle
        double baseline_ra = inr + (outr - inr) * (d.baseline - d.ylb)  / (d.yub - d.ylb);
        QRectF cr(c.x() - baseline_ra, c.y() - baseline_ra, 2 * baseline_ra, 2 * baseline_ra); // circle bounding rect
        p->drawEllipse(cr);

        if(d.show_origin) {
            pe.setColor(Qt::black);
            p->setPen(pe);
            double a0 = m_to_rad(d.a0);
            QPointF p0(R - baseline_ra * cos(a0) - inr / 10, R - baseline_ra * sin(a0));
            QPointF p1(p0);
            p1.setX(p1.x() + 2 * inr / 8.0);
            p->drawLine(p0, p1);
            p->setPen(txtpen);
            p1.setX((p0.x() + p1.x())  / 2);
            p->drawText(p1, QString::number(d.baseline));
        }
    }

    if(d.show_bounds) {
        double rlb = inr + (outr - inr) * (d.ylb)  / (d.yub - d.ylb);
        double rub = inr + (outr - inr) * (d.yub)  / (d.yub - d.ylb);
        QRectF cr(c.x() - inr, c.y() - inr, 2 * inr, 2 * inr); // circle bounding rect
        pe.setColor(Qt::magenta);
        p->drawEllipse(cr);
        cr.setRect(c.x() - outr, c.y() - outr, 2 * outr, 2 * outr);
        p->drawEllipse(cr);

        // values
        const double & aspan = m_to_rad(d.a1 - d.a0);
        const double & atxtlb = d.a0 + aspan / 4.0;
        const double &atxtub = d.a0 + aspan / 4.0;

        p->drawText(QPointF(R - inr * cos(atxtlb), R - inr * sin(atxtlb)), QString::number(d.ylb));
        p->drawText(QPointF(R - outr * cos(atxtub), R - outr * sin(atxtub)), QString::number(d.yub));

    }

    if(d.zoomer && d.zoomer->inZoom()) {
        QPen rpen(Qt::lightGray, 0.0);
        p->setPen(rpen);
        p->drawRect(d.zoomer->zoomArea());
    }

    QPen rpen(Qt::yellow, 0.0);
    p->setPen(rpen);
    p->drawRect(rect);

}

// R distance from center to side of bounding square (maximum radius)
QPointF* QuCircularPlotEngine::m_get_points(const QuCircularPlotCurve *c,
                                            const double &R,
                                            const double xmin, const double xmax,
                                            const double ymin, const double ymax,
                                            const double inr, const double outr) {
    QPointF *points = new QPointF[c->size()];
    double aspan = m_to_rad(d.a1 - d.a0); // angle span. default: 360 deg

    const QVector<double>& xv = c->x_data();
    const QVector<double>& yv = c->y_data();
    for(int i = 0; i < c->size(); i++) {
        const double &x = xv[i];
        double y = d.oob_xform->y_transform(yv[i], d.ylb, d.yub, d.ymin, d.ymax);
        const double &a = m_to_rad(d.a0) + (x - xmin) * aspan / (xmax - xmin);
        double dr;
        if(y <= d.yub && y >= d.ylb)
            dr = (y - d.ylb) * (outr - inr) / (d.yub - d.ylb);
        else if(y < d.ylb)
            dr = -inr + inr * (y - ymin) / (d.ylb - ymin);
        else
            dr = outr + (R - outr) * (ymax - y) / (ymax - d.yub);
        points[i] = QPointF(R - (inr + dr) * cos(a), R - (inr + dr) * sin(a));
//        pretty_pri("x %f -> angle %f y %f P(%f,%f) dr %f [inr %f outr %f] Y[%f-%f] curves[y:%f Y:%f]",
//                   xv[i], m_to_deg(a), y, points[i].x(), points[i].y(), dr, inr, outr, d.ylb, d.yub,
//                   ymin, ymax);
    }
    return points;
}
void QuCircularPlotEngine::setFont(const QFont &f) {
    d.font = f;
    recalculateTxtFont();
}

QSizeF QuCircularPlotEngine::minimumSize()  {
    return QSizeF(100,100);
}

void QuCircularPlotEngine::setOutOfBoundsTransform(OOBTransform *c) {
    if(d.oob_xform)
        delete d.oob_xform;
    d.oob_xform = c;
}

bool QuCircularPlotEngine::showValues() const {
    return d.show_values;
}

bool QuCircularPlotEngine::showPoints() const {
    return d.show_points;
}

bool QuCircularPlotEngine::showCurves() const   {
    return d.show_curves;
}

bool QuCircularPlotEngine::showXAxis() const {
    return d.show_xax;
}

int QuCircularPlotEngine::showYAxes() const {
    return d.y_axes;
}

bool QuCircularPlotEngine::showBounds() const {
    return d.show_bounds;
}

void QuCircularPlotEngine::setYLowerBound(double m) {
    d.y_autoscale = false;
    d.ylb = m;
}

void QuCircularPlotEngine::setYUpperBound(double m) {
    d.y_autoscale = false;
    d.yub = m;
}

void QuCircularPlotEngine::setXLowerBound(double m) {
    d.x_autoscale = false;
    d.xlb = m;
}

void QuCircularPlotEngine::setXUpperBound(double m) {
    d.x_autoscale = false;
    d.xub = m;
}

void QuCircularPlotEngine::setXAutoscaleEnabled(bool en) {
    d.x_autoscale = en;
}

void QuCircularPlotEngine::setYAutoscaleEnabled(bool en) {
    d.y_autoscale = en;
    if(en) {
        foreach(QuCircularPlotCurve *c, d.curves)
            c->minmax_update();
    }
}

void QuCircularPlotEngine::setRadiusFactor(float f) {
    d.radius_factor = f;
}

void QuCircularPlotEngine::setInnerRadiusFactor(float f) {
    d.inner_radius_factor = f;
}

void QuCircularPlotEngine::setOuterRadiusFactor(float f) {
    d.outer_radius_factor = f;
}

void QuCircularPlotEngine::contextMenuEvent(const QPointF &pos) {

}

void QuCircularPlotEngine::mousePressEvent(const QPointF &pos) {

}

void QuCircularPlotEngine::mouseMoveEvent(const QPointF &pos) {
}

void QuCircularPlotEngine::mouseReleaseEvent(const QPointF &pos) {
}

void QuCircularPlotEngine::mouseDoubleClickEvent(const QPointF &pos)
{

}

double QuCircularPlotEngine::minimum() const {
    return 0.0;
}

double QuCircularPlotEngine::maximum() const {
    return 0.0;
}

float QuCircularPlotEngine::radiusFactor() const {
    return d.radius_factor;
}

float QuCircularPlotEngine::innerRadiusFactor() const {
    return d.inner_radius_factor;
}

float QuCircularPlotEngine::outerRadiusFactor() const {
    return d.outer_radius_factor;
}


constexpr double QuCircularPlotEngine::m_to_rad(double deg) {
    return 2.0 * M_PI * deg / 360.0;
}

constexpr double QuCircularPlotEngine::m_to_deg(double rad) {
    return 360.0 * rad / (2 * M_PI);
}

void QuCircularPlotEngine::setShowValues(bool show) {
    d.show_values = show;
}

void QuCircularPlotEngine::setShowPoints(bool show) {
    d.show_points = show;

}

void QuCircularPlotEngine::setShowCurves(bool show) {
    d.show_curves = show;
}

void QuCircularPlotEngine::setShowYAxes(int count) {
    d.y_axes = count;
}

void QuCircularPlotEngine::setShowBounds(bool show) {
    d.show_bounds = show;
}

void QuCircularPlotEngine::recalculateTxtFont() {
    double R = qMin(d.bounding_r.width(), d.bounding_r.height()) / 2.0;
    d.scaled_font = d.font;
    QFontMetrics fm(d.scaled_font);
    while(fm.height() > R / 10.0) {
        d.scaled_font.setPointSizeF(d.scaled_font.pointSizeF() - 1.0);
        fm = QFontMetrics(d.scaled_font);
    }
    pretty_pri("recalculating font size from %f to %f", d.font.pointSizeF(),
               d.scaled_font.pointSizeF());
}
