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
#include "quzoomer.h"

class QStyleOptionGraphicsItem;
class QPainter;
class OOBTransform;

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

    OOBTransform *oob_xform;
    bool show_values, show_points, show_bounds; // default false
    bool show_curves, show_xax, show_origin; // default: true
    int y_axes; // default: show 0 Y axes

    QuZoomer *zoomer;
};

class QuCircularPlotEngine : public QObject
{
    Q_OBJECT
public:
    QuCircularPlotEngineData d;

    explicit QuCircularPlotEngine(const QFont &f, QuZoomer *zoomer);
    virtual  ~QuCircularPlotEngine();

    void contextMenuEvent(const QPointF& pos);
    void mousePressEvent(const QPointF& pos);
    void mouseMoveEvent(const QPointF& pos);
    void mouseReleaseEvent(const QPointF& pos);
    void mouseDoubleClickEvent(const QPointF& pos);

    double minimum() const;
    double maximum() const;

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

    void setShowValues(bool show);
    void setShowPoints(bool show);
    void setShowCurves(bool show);
    void setShowYAxes(int count);
    void setShowBounds(bool show);

    void recalculateTxtFont();

signals:
    void minimumChanged(double m);
    void maximumChanged(double M);

private:

    QPointF *m_get_points(const QuCircularPlotCurve *c,
                          const double &R,
                          const double xmin, const double xmax,
                          const double ymin, const double ymax,
                          const double inr, const double outr);

    constexpr double m_to_rad(double deg);
    constexpr double m_to_deg(double rad);
    void m_get_bounds_from_curves(double *x, double *X, double *y, double *Y, int *max_datasiz) const;
    void m_get_bounds(double *x, double *X, double *y, double *Y, double *inr, double *outr, const double &R) const;
private slots:

};

#endif // QUCIRCULARPLOTENGINE_H
