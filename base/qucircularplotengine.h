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
class OOBDistort;
class QuZoomEvents;
class QuCircularPlotCurveSelectionEvents;
class QuZoomer;
class QuCircularPlotDrawable_I;

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
    bool x_autoscale, y_autoscale, scale_inverted;
    int maxdatasiz;

    double radius, radius_factor, inner_radius_factor, outer_radius_factor;

    float crv_edit_r, csel_pt_r; // selection circle and point

    OOBDistort *oob_xform;
    bool show_values, show_points, show_bounds; // default false
    bool show_curves, show_xax, show_origin; // default: true
    int y_axes; // default: show 0 Y axes

    QuZoomer *zoomer;
    QuCircularPlotCurveSelectionEvents *selection_ev;
    QuZoomEvents *zoom_ev;

    QColor bgcolor, axcolor;

    QMultiMap<int, QuCircularPlotDrawable_I *> drawables;
};

class QuCircularPlotEngine : public QObject, public QuCircularPlotCurveListener
{
    Q_OBJECT
public:
    QuCircularPlotEngineData d;

    explicit QuCircularPlotEngine(QObject *parent, const QFont &f, QuZoomer *zoomer, QuCircularPlotCurveSelectionEvents* selev, QuZoomEvents* ze);
    virtual  ~QuCircularPlotEngine();

    QuCircularPlotCurve* addCurve(const QString& src);
    QuCircularPlotCurve* findCurve(const QString& src) const;

    QuZoomer *zoomer() const;
    QuZoomEvents *zoomEvents() const;

    QObject *plot() const;

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

    void setOutOfBoundsTransform(OOBDistort *c);

    bool showValues() const;
    bool showPoints() const;
    bool showCurves() const;
    bool showXAxis() const;
    int  showYAxes() const;
    bool showBounds() const;
    bool scaleInverted() const;

    float curveEditRadius() const;
    float curveSelectedPointRadius() const;

    void addDrawable(QuCircularPlotDrawable_I *dra);
    QList<QuCircularPlotDrawable_I *> drawables() const;

public slots:
    void setYLowerBound(double m);
    void setYUpperBound(double m);
    void setXLowerBound(double m);
    void setXUpperBound(double m);
    void setXBounds(double xm, double xM);
    void setYBounds(double ym, double yM);
    void setXAutoscaleEnabled(bool en);
    void setYAutoscaleEnabled(bool en);
    void setScaleInverted(bool inve);

    void setRadiusFactor(float f);
    void setInnerRadiusFactor(float f);
    void setOuterRadiusFactor(float f);

    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);

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

    // selection related methods
    void select(const QPointF& p);
    void moveSelection(const double &dx, const double &dy);
    void clearSelection();

signals:
    void minimumChanged(double m);
    void maximumChanged(double M);
    void selected(const QPointF& p);
    void closestPoint(const QList<QuCircularPlotCurve *>& curves, int idx, const QPointF& pt);
    void dirty(); // trigger update on item / widget
    void selectionChanged(QuCircularPlotCurve *c, int idx, double new_val);
    void selectionChanged(QObject *o, const QString& curve, int idx, double new_val);

    /*!
     * \brief selectedYChanged is emitted when a selection on one (or more) curve(s) is active and the
     *        y value for the selected curve index changes
     * \param c the curve for which the y value changed at the selected index
     *
     * \par Note
     * You can get the selected index through QuCircularPlotCurve::selected and the
     * corresponding value accessing QuCircularPlotCurve::y_data at that index
     */
    void selectedYChanged(QuCircularPlotCurve *c);

private:

    void m_paint(QPainter *p, double R, double inr, double outr, const QRectF &rect, QWidget *widget);
    QPointF *m_get_points(const QuCircularPlotCurve *c,
                          const double &R,
                          const double xmin, const double xmax,
                          const double ymin, const double ymax,
                          const double inr, const double outr);

    constexpr double m_to_rad(double deg);
    constexpr double m_to_deg(double rad);
    void m_get_bounds_from_curves(double *x, double *X, double *y, double *Y, int *max_datasiz) const;
    void m_get_bounds_distorted(double *x, double *X, double *y, double *Y) const;
    double m_map_yp2y(const QPointF &p, const double &ylb, const double &yub, const double& R) const;
    void m_update_scales();
    void m_autoscale_enabled();

private slots:
    void m_zoom_changed();

    // QuCircularPlotCurveListener interface
private:
    void onCurveBoundsChanged();
};

#endif // QUCIRCULARPLOTENGINE_H
