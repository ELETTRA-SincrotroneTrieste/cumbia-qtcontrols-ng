#include "qucircularplotengine.h"
#include "out_of_bounds_distortions.h"
#include "quzoomevents.h"
#include "quzoomer.h"
#include "qucircularplot_drawable_i.h"
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
#include <QSet>

#define CHECK_MARK "✓"


QuCircularPlotEngineData::QuCircularPlotEngineData()
    : bounding_r(0, 0, 100, 100), a0(0), a1(360.0),
    xlb(0.0), xub(0.0), ylb(0.0), yub(0.0),
    xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), baseline(500.0),
    x_autoscale(true), y_autoscale(true), scale_inverted(false),
    show_values(false), show_points(false), show_bounds(true),
    show_curves(true),
    show_xax(true), show_origin(true) , y_axes(0) {
    radius_factor = 0.5;
    inner_radius_factor = 0.2;
    outer_radius_factor = 0.8;
    crv_edit_r = 3.5;
    csel_pt_r = 1.2;
    maxdatasiz = 0;
    oob_xform = new OOBLogDistort;
    //    oob_xform = new OOBTransform;
    bgcolor = QColor(Qt::white);
    axcolor = QColor(Qt::lightGray);
}

QuCircularPlotEngine::QuCircularPlotEngine(QObject *parent, const QFont& f, QuZoomer *zoomer, QuCircularPlotCurveSelectionEvents* selev, QuZoomEvents *ze)
    : QObject{parent}  {
    d.font = f;
    d.zoomer = zoomer;
    d.selection_ev = selev;
    d.zoom_ev = ze;
    connect(d.zoom_ev, SIGNAL(zoomRectChanged(QRectF)), d.zoomer, SLOT(setZoomRect(QRectF)));
    connect(d.zoom_ev, SIGNAL(moveRect(QPointF,QPointF)), d.zoomer, SLOT(moveZoom(QPointF,QPointF)));
    connect(d.zoom_ev, SIGNAL(unzoom()), d.zoomer, SLOT(unzoom()));
    connect(d.zoom_ev, SIGNAL(zoomRectChanging(QPointF,QPointF)), d.zoomer, SLOT(zoomRectChanging(QPointF,QPointF)));
    connect(d.zoom_ev, SIGNAL(clicked(QPointF,Qt::MouseButton,Qt::KeyboardModifiers)),
            d.zoomer, SLOT(map(QPointF,Qt::MouseButton,Qt::KeyboardModifiers)));
    connect(d.selection_ev, SIGNAL(selected(QPointF)), this, SLOT(select(QPointF)));
    connect(d.selection_ev, SIGNAL(deselected()), this, SLOT(clearSelection()));
    connect(d.selection_ev, SIGNAL(selectionMoved(double,double)), this, SLOT(moveSelection(double,double)));
    connect(d.zoomer, SIGNAL(zoomChanged()), this, SLOT(m_zoom_changed()));
}

QuCircularPlotEngine::~QuCircularPlotEngine() {
    delete d.zoom_ev;
    delete d.zoomer;
}

QuCircularPlotCurve *QuCircularPlotEngine::addCurve(const QString& src) {
    if(d.curves.contains(src))
        delete d.curves[src];
    d.curves[src] = new QuCircularPlotCurve(src, this);
    QuCircularPlotConfigurator().configure(d.curves[src], d.curves.size());
    return d.curves[src];
}

QuCircularPlotCurve *QuCircularPlotEngine::findCurve(const QString &src) const {
    return d.curves.contains(src) ? d.curves[src] : nullptr;
}

void QuCircularPlotEngine::setData(const QString &src, const QVector<double> &xdata, const QVector<double> &ydata){
    if(!d.curves.contains(src)) {
        QuCircularPlotConfigurator pco;
        d.curves[src] = new QuCircularPlotCurve(src, this);
        pco.configure(d.curves[src], d.curves.size());
    }
    // curve->setData invokes onBoundsChanged if either min or max change
    QuCircularPlotCurve *c = d.curves[src];
    double v = 0.0;
    int i = c->selected();
    bool save = i > -1 && i > c->size() && i > ydata.size();
    if(save)
        v = c->y_data()[i];
    c->setData(xdata, ydata); // check onBoundsChanged
    if(save && v != ydata[i])
        emit selectedYChanged(c);
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

void QuCircularPlotEngine::m_get_bounds_distorted(double *x, double *X,
                                        double *y, double *Y) const
{
    d.oob_xform->bounds_distort(x, X, y, Y, d.xlb, d.xub, d.ylb, d.yub, d.xmin, d.xmax, d.ymin, d.ymax);
}

// convert a y coordinate of a point in logical coords to an y value according to
// upper and lower bounds and radius
double QuCircularPlotEngine::m_map_yp2y(const QPointF &p, const double& ylb, const double& yub, const double &R) const {
    const QPointF &pc = d.bounding_r.center();
    const double l = sqrt((p.y() - pc.y()) * (p.y() - pc.y()) + (p.x() - pc.x()) * (p.x() - pc.x()));
    // span in logical coords
    const double &s = (R * d.outer_radius_factor)  - R * d.inner_radius_factor;
    qDebug() << __PRETTY_FUNCTION__ << p << "y span" << yub - ylb << "s" << s  << "R" << R << "inner" <<  d.inner_radius_factor <<
        "outer factor" << d.outer_radius_factor;
    double len = !d.scale_inverted ? l - R * d.inner_radius_factor : R * d.outer_radius_factor - l;
    return len * (yub - ylb) / s + ylb;
}

void QuCircularPlotEngine::select(const QPointF &pt) {
    QPointF p = d.zoomer->transform().inverted().map(pt);
    QPointF closest;
    int idx = -1;
    QSet<QuCircularPlotCurve *> intersecting_crvs;
    const double &R = qMin(d.bounding_r.width(), d.bounding_r.height()) / 2.0;
    const double &inr = R * d.inner_radius_factor, &outr = R * d.outer_radius_factor;
    double xmin, xmax, ymin, ymax;
    m_get_bounds_distorted(&xmin, &xmax, &ymin, &ymax);
    double min = -1.0;
    foreach( QuCircularPlotCurve *c, d.curves) {
        QPointF diff;
        const QVector<double> &vx = c->x_data();
        const QVector<double> &vy = c->y_data();
        const QPointF *points = m_get_points(c, R, xmin, xmax, ymin, ymax, inr, outr);
        for(int i = 0; i < vx.size() && i < vy.size(); i++) {
            diff = points[i] - p;
            if(min < 0 || min > diff.manhattanLength()) {
                min = diff.manhattanLength();
                idx = i;
                closest = points[i];
                intersecting_crvs.insert(c);
            }
        }
        delete [] points;

    }
    qDebug() << __PRETTY_FUNCTION__ << "clicked" << p << "curves" << intersecting_crvs <<
        "idx" << idx << "at logical closest point coord " << closest;
    foreach(QuCircularPlotCurve *c, intersecting_crvs) {
        c->select(idx);
        emit selectedYChanged(c);
        printf("%s ( \e[1;32m %f\e[0m,\e[1;32m%f\e[0m ), ", qstoc(c->source()), c->x_data().at(idx), c->y_data().at(idx));
    }
    printf("\n");
    d.selection_ev->select(QRectF(closest.x() - d.crv_edit_r, closest.y() - d.crv_edit_r, 2 * d.crv_edit_r, 2 * d.crv_edit_r));
    emit selected(p);
    emit closestPoint(QList<QuCircularPlotCurve *>(intersecting_crvs.begin(), intersecting_crvs.end()), idx, closest);
    emit dirty();
}

void QuCircularPlotEngine::moveSelection(const double &dx, const double& dy) {
    qDebug() << __PRETTY_FUNCTION__ << dx << dy;
    const QPointF& pc = d.selection_ev->selectionArea().center();
    QRectF sr = d.selection_ev->selectionArea();
    QPointF p (pc.x() + dx, pc.y() + dy);
    sr.moveCenter(p);
    d.selection_ev->select(sr);
    double R = qMin(d.bounding_r.width(), d.bounding_r.height()) / 2.0;
    double y = m_map_yp2y(p, d.ylb, d.yub, R);
    printf("QuCircularPlotEngine::m_selection_moved \e[0;32mselection moved\e[0m -->  pt.y %f mapped to y %f\n", p.y(), y);
    foreach(QuCircularPlotCurve *c, d.curves) {
        if(c->y_data().size() > c->selected()) {
            emit selectionChanged(c, c->selected(), y);
            emit selectionChanged(parent(), c->source(), c->selected(), y);
            emit selectedYChanged(c);
        }
    }
    emit dirty();
}

void QuCircularPlotEngine::clearSelection() {
    foreach(QuCircularPlotCurve *c, d.curves) {
        if(c->selected()) {
            c->deselect();
            emit selectedYChanged(c);
        }
    }
    d.selection_ev->select(QRectF());
    emit dirty();
}

void QuCircularPlotEngine::m_zoom_changed() {
    // make adjustments to zoom dependent objects
    d.selection_ev->setTransform(d.zoomer->calculateTransform(d.bounding_r));
}

void QuCircularPlotEngine::m_paint(QPainter *p, double R, double inr,  double outr, const QRectF &rect, QWidget *widget) {
    const QPointF& c = rect.center();
    double xmin, xmax, ymin, ymax;
    m_get_bounds_distorted(&xmin, &xmax, &ymin, &ymax);



    QPen txtpen(Qt::gray, 0.0);
    if(d.show_values || d.show_bounds) {
        p->setFont(d.scaled_font);
    }

    QPen pe(d.axcolor, 0.0);
    p->setPen(pe);

    // 1. draw background, before drawables
    if(d.bgcolor.isValid()) {
        p->setBrush(d.bgcolor);
        p->drawEllipse(d.bounding_r);
        p->setBrush(Qt::NoBrush);
    }

    // 2. draw drawables with z <=0
    QList<QuCircularPlotDrawable_I *> _drawables = d.drawables.values();
    foreach (QuCircularPlotDrawable_I *drawable, _drawables) {
        if(drawable->z() <= 0 && drawable->scales())
            drawable->draw(p, this, inr, outr, rect, widget);
    }

    // 3. draw bounds if enabled
    if(d.show_bounds) {
        QRectF cr(c.x() - outr, c.y() - outr, 2 * outr, 2 * outr); // outer
        p->drawEllipse(cr);
        cr.setRect(c.x() - inr, c.y() - inr, 2 * inr, 2 * inr); // inner
        pe.setColor(Qt::magenta);
        p->drawEllipse(cr);

        // values
        const double & aspan = m_to_rad(d.a1 - d.a0);
        const double & atxtlb = d.a0 + aspan / 4.0;
        const double &atxtub = d.a0 + aspan / 4.0;

        p->drawText(QPointF(R - inr * cos(atxtlb), R - inr * sin(atxtlb)), QString::number(!d.scale_inverted ? d.ylb : d.yub));
        p->drawText(QPointF(R - outr * cos(atxtub), R - outr * sin(atxtub)), QString::number(!d.scale_inverted ? d.yub : d.ylb));
    }

    // 4. draw baseline
    if(d.baseline >= d.ylb && d.baseline <= d.yub) {
        // draw circle
        double yp = (outr - inr) * (d.baseline - d.ylb)  / (d.yub - d.ylb);
        double baseline_ra = !d.scale_inverted ? inr + yp : outr - yp;
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

    // 5. draw curves
    foreach(const QuCircularPlotCurve* c, d.curves) {
        QPointF *points = this->m_get_points(c, R, xmin, xmax, ymin, ymax, inr, outr);
        if(c->selected() >= 0 && c->selected() < c->size()) {
            // draw edit area
            if(c->isEditable()) {
                if(d.selection_ev->selectionArea().isValid()) {
                    p->setBrush(QColor("orange"));
                    p->setPen(QPen(QColor("red"), 0.0));
                    p->drawEllipse(d.selection_ev->selectionArea());
                }
            }// draw selection point
            p->setBrush(QColor("yellow"));
            p->drawEllipse(points[c->selected()], d.csel_pt_r, d.csel_pt_r);
            p->setBrush(Qt::NoBrush);
        }
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

    // 6. draw drawables with positive z
    foreach (QuCircularPlotDrawable_I *drawable, _drawables) {
        if(drawable->z() > 0 && drawable->scales())
            drawable->draw(p, this, inr, outr, rect, widget);
    }
}

void QuCircularPlotEngine::paint(QPainter *p, const QRectF &rect, QWidget *wi) {
    p->setRenderHint(QPainter::Antialiasing, true);
    bool in_zoom = d.zoomer->inZoom();
    float xr = 0.0f, yr = 0.0f, xp = 0.0, yp = 0.0f;

    double R = qMin(rect.width(), rect.height()) / 2.0;
    double inr =  R * d.inner_radius_factor, outr = R * d.outer_radius_factor;

    QList<QuCircularPlotDrawable_I *> _drawables = d.drawables.values();
    foreach (QuCircularPlotDrawable_I *drawable, _drawables) {
        if(drawable->z() <= 0 && !drawable->scales())
            drawable->draw(p, this, inr, outr, rect, wi);
    }

    if(in_zoom) {
        p->save();
        const QTransform& T = d.zoomer->calculateTransform(rect);
        xr = T.m11();
        yr = T.m22();
        xp = -T.m31() / xr;
        yp = -T.m32() / yr;
        QTransform pt = p->combinedTransform(); // current painter transform
        p->setClipRect(rect);
        p->setTransform(T * pt);
    }

    // draw
    m_paint(p, R, inr, outr, rect, wi);



    QPen pen(Qt::cyan, 0.0);
    p->setPen(pen);
    p->drawRect(rect);

    if(d.zoomer->rectChanging()) {
        QPen rpen(Qt::lightGray, 0.0);
        rpen.setWidthF(0.0);
        rpen.setColor(Qt::darkGray);
        p->setPen(rpen);
        p->drawRect(d.zoomer->zoomArea());
    }

    if(d.zoomer->inZoom())
        p->restore();

    foreach (QuCircularPlotDrawable_I *drawable, _drawables) {
        if(drawable->z() <= 0 && !drawable->scales())
            drawable->draw(p, this, inr, outr, rect, wi);
    }

    if(d.zoomer->inZoom()) { // restore painter and draw a zoom indicator
        QColor c(Qt::lightGray);
        QPen pe(c, 0.0);
        pe.setStyle(Qt::DashLine);
        p->setPen(pe);
        p->drawRect(rect);
        pe.setStyle(Qt::SolidLine);
        p->setPen(pe);
        c = c.lighter(120);
        c.setAlpha(120);
        p->setBrush(c);
        float hscroll_len = rect.width() / xr;
        float vscroll_len = rect.height() / yr;
        const float scrollw = qMin(rect.width(), rect.height()) / 40;
        p->drawRect(xp, rect.height() - scrollw, hscroll_len, scrollw);
        p->drawRect(rect.width() - scrollw, yp - scrollw, scrollw, vscroll_len);
        qDebug() << __PRETTY_FUNCTION__ << "scrollbar" << QRectF(xp, rect.height() - scrollw, hscroll_len, scrollw);
    }
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
        double y = d.oob_xform->y_distort(yv[i], d.ylb, d.yub, d.ymin, d.ymax);
        const double &a = m_to_rad(d.a0) + (x - xmin) * aspan / (xmax - xmin);
        double dr;
        if(y <= d.yub && y >= d.ylb) {

            dr = (y - d.ylb) * (outr - inr) / (d.yub - d.ylb);
        }
        else if(y < d.ylb) {
            dr = -(d.ylb - y) * inr / (d.ylb - ymin);
            printf("QuCircularPlotEngine::m_get_points: y is %f ymin is %f ylb is %f dr is \e[1;31m%f\e[0m\n",
                   y, ymin, d.ylb, dr);
        }
        else {
            dr = outr - inr + (R - outr) * (y - d.yub) / (ymax - d.yub);
            printf("QuCircularPlotEngine::m_get_points: y is %f ymax is %f yub is %f dr is \e[0;31m%f\e[0m\n",
                   y, ymax, d.yub, dr);
        }
        double r = !d.scale_inverted ? inr + dr : outr - dr;
        points[i] = QPointF(R - r * cos(a), R - r * sin(a));
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

void QuCircularPlotEngine::setOutOfBoundsTransform(OOBDistort *c) {
    if(d.oob_xform)
        delete d.oob_xform;
    d.oob_xform = c;
    emit dirty();
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

bool QuCircularPlotEngine::scaleInverted() const {
    return d.scale_inverted;
}

void QuCircularPlotEngine::setCurveEditRadius(float r) {
    d.crv_edit_r = r;
    emit dirty();
}

void QuCircularPlotEngine::setCurveSelectionPointRadius(float r) {
    d.csel_pt_r = r;
    emit dirty();
}

float QuCircularPlotEngine::curveEditRadius()  const {
    return d.crv_edit_r;
}

float QuCircularPlotEngine::curveSelectedPointRadius()  const {
    return d.csel_pt_r;
}

void QuCircularPlotEngine::addDrawable(QuCircularPlotDrawable_I *dra) {
    d.drawables.insert(dra->z(), dra);
}

QList<QuCircularPlotDrawable_I *> QuCircularPlotEngine::drawables() const {
    return d.drawables.values();
}

void QuCircularPlotEngine::onCurveBoundsChanged() {
    m_get_bounds_from_curves(&d.xmin, &d.xmax, &d.ymin, &d.ymax, &d.maxdatasiz);
    if(d.x_autoscale) {
        d.xlb = d.xmin; d.xub = d.xmax;
        setXBounds(d.xlb, d.xub);
    }
    if(d.y_autoscale) {
        d.ylb = d.ymin; d.yub = d.ymax;
        setYBounds(d.ylb, d.yub);
    }
}

// d.ymin, d.ymax, d.xmin, d.xmax updated upon curve data change
void QuCircularPlotEngine::m_update_scales() {
    if(d.y_autoscale)  {
        setYBounds(d.ylb, d.yub);
    }
    if(d.x_autoscale) {
        setXBounds(d.xlb, d.xub);
    }
}

void QuCircularPlotEngine::m_autoscale_enabled() {
    foreach(QuCircularPlotCurve *c, d.curves)
        c->minmax_update();
    m_get_bounds_from_curves(&d.xlb, &d.xub, &d.ylb, &d.yub, &d.maxdatasiz);
    m_update_scales();
    emit dirty();
}

void QuCircularPlotEngine::setXAutoscaleEnabled(bool en) {
    d.x_autoscale = en;
    if(en) m_autoscale_enabled();
}

void QuCircularPlotEngine::setYAutoscaleEnabled(bool en) {
    d.y_autoscale = en;
    if(en) m_autoscale_enabled();
}

void QuCircularPlotEngine::setYLowerBound(double m) {
    d.y_autoscale = false;
    d.ylb = m;
    emit dirty();
}

void QuCircularPlotEngine::setYUpperBound(double m) {
    d.y_autoscale = false;
    d.yub = m;
    emit dirty();
}

void QuCircularPlotEngine::setXLowerBound(double m) {
    d.x_autoscale = false;
    d.xlb = m;
    emit dirty();
}

void QuCircularPlotEngine::setXUpperBound(double m) {
    d.x_autoscale = false;
    d.xub = m;
    emit dirty();
}

void QuCircularPlotEngine::setXBounds(double xm, double xM) {
    d.xlb = xm;
    d.xub = xM;
    emit dirty();
}

void QuCircularPlotEngine::setYBounds(double ym, double yM) {
    d.ylb = ym;
    d.yub = yM;
    emit dirty();
}


void QuCircularPlotEngine::setScaleInverted(bool inve) {
    d.scale_inverted = inve;
    clearSelection();
    emit dirty();
}

void QuCircularPlotEngine::setRadiusFactor(float f) {
    d.radius_factor = f;
    emit dirty();
}

void QuCircularPlotEngine::setInnerRadiusFactor(float f) {
    d.inner_radius_factor = f;
    emit dirty();
}

void QuCircularPlotEngine::setOuterRadiusFactor(float f) {
    d.outer_radius_factor = f;
    emit dirty();
}

double QuCircularPlotEngine::yLowerBound() const {
    return d.ylb;
}

double QuCircularPlotEngine::yUpperBound() const {
    return d.yub;
}

double QuCircularPlotEngine::xLowerBound() const {
    return d.xlb;
}

double QuCircularPlotEngine::xUpperBound() const {
    return d.xub;
}

QColor QuCircularPlotEngine::background() const {
    return d.bgcolor;
}

QColor QuCircularPlotEngine::axesColor() const {
    return d.axcolor;
}

QuZoomer *QuCircularPlotEngine::zoomer() const {
    return d.zoomer;
}

QuZoomEvents *QuCircularPlotEngine::zoomEvents() const {
    return d.zoom_ev;
}

QObject *QuCircularPlotEngine::plot() const {
    return parent();
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
}

void QuCircularPlotEngine::setBackground(const QColor &c) {
    d.bgcolor = c;
}

void QuCircularPlotEngine::setAxesColor(const QColor &c) {
    d.axcolor = c;
}

