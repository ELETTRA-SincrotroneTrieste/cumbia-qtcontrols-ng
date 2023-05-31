#ifndef QUCIRCULARPLOTENGINE_H
#define QUCIRCULARPLOTENGINE_H

#include <QFont>
#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QVector>
#include <QMap>
#include <math.h>
#include "qucircularplotcurve.h"
#include "qucircularplotcurveselectionevents.h"

class QStyleOptionGraphicsItem;
class QPainter;
class OOBTransform;
class QuZoomEvents;
class QuCircularPlotCurveSelectionEvents;
class QuZoomer;

class QuCircularPlotEngineData {
public:
    QuCircularPlotEngineData();
    QRectF bounding_r;
    QFont font, scaled_font;
    QMap<QString, QuCircularPlotCurve *> curves;
    double a0, a1; // angle start, angle end
    double xlb, xub, ylb, yub; // "axis" bounds, either manual or auto
    double xmin, xmax, ymin, ymax; // min and max from curves
    double baseline; // x axis pos, default at y = 0.0
    bool x_autoscale, y_autoscale;
    int maxdatasiz;

    double radius, radius_factor, inner_radius_factor, outer_radius_factor;

    float crv_edit_r, csel_pt_r; // selection circle and point

    OOBTransform *oob_xform;
    bool show_values, show_points, show_bounds; // default false
    bool show_curves, show_xax, show_origin; // default: true
    int y_axes; // default: show 0 Y axes

    QuZoomer *zoomer;
    QuCircularPlotCurveSelectionEvents *selection_ev;
    QuZoomEvents *zoom_ev;

    QColor bgcolor, axcolor;
};

class QuCircularPlotEngine : public QObject
{
    Q_OBJECT
public:
    QuCircularPlotEngineData d;

    explicit QuCircularPlotEngine(const QFont &f, QuZoomer *zoomer, QuCircularPlotCurveSelectionEvents* selev, QuZoomEvents* ze);
    virtual  ~QuCircularPlotEngine();

    void contextMenuEvent(const QPointF& pos);
    void mousePressEvent(const QPointF& pos);
    void mouseMoveEvent(const QPointF& pos);
    void mouseReleaseEvent(const QPointF& pos);
    void mouseDoubleClickEvent(const QPointF& pos);

    double minimum() const;
    double maximum() const;

    QuZoomer *zoomer() const;
    QuZoomEvents *zoomEvents() const;

    double yLowerBound() const;
    double yUpperBound() const;
    double xLowerBound() const;
    double xUpperBound() const;

    QColor background() const;
    QColor axesColor() const;

    /*!
     * \brief radiusFactor is a factor from 0 to 1 indicating where to draw the circle
     *        in the available area
     * \return the value of the radius factor.
     * \par Default value
     * The default value is 0.4
     */
    float radiusFactor() const;

    float innerRadiusFactor() const;
    float outerRadiusFactor() const;

    void paint(QPainter *p, const QRectF &rect, QWidget *widget);
    void setFont(const QFont& f);

    QSizeF minimumSize();

    void setOutOfBoundsTransform(OOBTransform *c);

    bool showValues() const;
    bool showPoints() const;
    bool showCurves() const;
    bool showXAxis() const;
    int  showYAxes() const;
    bool showBounds() const;


    float curveEditRadius() const;
    float curveSelectedPointRadius() const;

public slots:
    void setYLowerBound(double m);
    void setYUpperBound(double m);
    void setXLowerBound(double m);
    void setXUpperBound(double m);
    void setXAutoscaleEnabled(bool en);
    void setYAutoscaleEnabled(bool en);

    void setRadiusFactor(float f);
    void setInnerRadiusFactor(float f);
    void setOuterRadiusFactor(float f);

    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);
    QuCircularPlotCurve* addCurve(const QString& src);
    QuCircularPlotCurve* findCurve(const QString& src) const;

    void setShowValues(bool show);
    void setShowPoints(bool show);
    void setShowCurves(bool show);
    void setShowYAxes(int count);
    void setShowBounds(bool show);

    void recalculateTxtFont();

    // colors
    void setBackground(const QColor& c);
    void setAxesColor(const QColor& c);

    void setCurveEditRadius(float r);
    void setCurveSelectionPointRadius(float r);

signals:
    void minimumChanged(double m);
    void maximumChanged(double M);
    void selected(const QPointF& p);
    void closestPoint(const QList<QuCircularPlotCurve *>& curves, int idx, const QPointF& pt);
    void dirty(); // trigger update on item / widget
private:

    void m_paint(QPainter *p, const QRectF &rect);
    QPointF *m_get_points(const QuCircularPlotCurve *c,
                          const double &R,
                          const double xmin, const double xmax,
                          const double ymin, const double ymax,
                          const double inr, const double outr);

    constexpr double m_to_rad(double deg);
    constexpr double m_to_deg(double rad);
    void m_get_bounds_from_curves(double *x, double *X, double *y, double *Y, int *max_datasiz) const;
    void m_get_bounds(double *x, double *X, double *y, double *Y, double *inr, double *outr, const double &R) const;
    double m_map_yp2y(const QPointF &p, const double &ylb, const double &yub, const double& R) const;

private slots:
    void m_point_selected(const QPointF& p);
    void m_selection_moved(const QPointF& p);
    void m_point_deselected();
    void m_zoom_changed();

private slots:

};

#endif // QUCIRCULARPLOTENGINE_H
