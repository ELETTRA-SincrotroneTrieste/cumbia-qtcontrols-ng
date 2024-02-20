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
#include "qucircularplotattributes.h"

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
    double xmin, xmax, ymin, ymax; // min and max from curves
    int maxdatasiz;

    QuZoomer *zoomer;
    QuCircularPlotCurveSelectionEvents *selection_ev;
    QuZoomEvents *zoom_ev;

    QMultiMap<int, QuCircularPlotDrawable_I *> drawables;
};

/*!
 * \brief Engine shared by either QuCircularPlotItem or QuCircularPlotWidget that stores configuration
 *        and does all the painting and zooming and stores the data
 *
 *  QuCircularPlotItem::engine and QuCircularPlotWidget::engine return a reference to their
 *  QuCircularPlotEngine, that shall be used by clients to access all operations, as well as
 *  signals and slots.
 *
 *  \par Plot configuration
 *  Plot settings are held by the *qucircularplot_attributes* public member named *a*
 *  QuCircularPlotEngine::att and QuCircularPlotEngine::attributes are convenience
 *  methods that return a (non const) reference to *a*.
 *  \see qucircularplot_attributes
 *
 *  \par Example
 *
 *  In this example we set up a QGraphicsPlotItem with one curve
 *
 *  \code
    QuCircularPlotI *ni = new QuCircularPlotI(nullptr, QSize(300, 300));
    QuCircularPlotEngine *pie = ni->engine();
    qucircularplot_attributes &a = pie->a;
    a.axes.y.autoscale = true;  // enable y axis autoscale
    a.geom.inner_radius_factor = 0.1; // widen inter axes area
    a.paint.colors.background = QColor(Qt::white);  // light color scheme
    ni->setObjectName("plot_" + s);
    ni->setFlag(QGraphicsItem::ItemIsMovable, true);
    scene->addItem(ni); // add the item to a (previously allocated) QGraphicsScene
    ni->setPos(10, 10);
    QuCircularPlotCurve *crv = pie->addCurve("curve 1");
    QVector<double> x{0,1,2,3,4,5}, y{0.0, 0.5, -0.5, 0.4, -0.4. 0.1};
    crv->setData(x, y);
    // or ni->setData("curve 1", x, y);
 *  \endcode
 */
class QuCircularPlotEngine : public QObject, public QuCircularPlotCurveListener
{
    Q_OBJECT
public:

    struct qucircularplot_attributes a; //!< plot configuration @see qucircularplot_attributes
    struct qucircularplot_attributes& att();
    struct qucircularplot_attributes& attributes() ;

    explicit QuCircularPlotEngine(QObject *parent,
                                  const QFont &f,
                                  const QSize& siz,
                                  QuZoomer *zoomer,
                                  QuCircularPlotCurveSelectionEvents* selev,
                                  QuZoomEvents* ze);
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

    void setSize(const QSizeF& s);
    void setRect(const QRectF& r);
    QRectF boundingRect() const;

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
    void clicked(const QPointF& p, const Qt::MouseButton butt, Qt::KeyboardModifiers mod);

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

private:
    QuCircularPlotEngineData d;
};

#endif // QUCIRCULARPLOTENGINE_H
