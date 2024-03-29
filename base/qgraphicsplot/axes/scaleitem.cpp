#include "scaleitem.h"
#include "scaleitemprivate.h"
#include "curve/scenecurve.h"
#include "scalelabelinterface.h"
#include "curve/curveitem.h"
#include <stdio.h>
#include <QGraphicsScene>
#include <QDateTime>
#include <QPainter>
#include "qgraphicsplotitem.h"

#include "qgraphicsplotmacros.h"
#include <math.h>
#include <QFont>
#include <QStyleOptionGraphicsItem>
#include <QtDebug>

ScaleItem::~ScaleItem() {
    qDebug() << __FUNCTION__ << "deleting ScaleItem " << objectName();
}

/** \brief Constructs a scale item with an Orientation, a reference to the PlotSceneWidget
  *        and an id aimed at identifying the axis.
  *
  * The PlotSceneWidget creates two ScaleItem objects for you, an x item and an y item.
  * They are laid out so that they cross each other in the middle
  * (see PlotSceneWidget::setOriginPosPercentage, PlotSceneWidget::setDefaultXAxisOriginPosPercentage
  * and PlotSceneWidget::setDefaultYAxisOriginPosPercentage)
  *
  * If you want to customize the scale labels, then subclass ScaleLabelInterface and provide a
  * method named label in order to tell the ScaleItem what text to draw in correspondance to a
  * specific value on the axis.
  * @see ScaleLabelInterface
  *
  * The ScaleItem is a QGraphicsObject which is added to the scene of the PlotSceneWidget.
  *
  * The ScaleItem::Id is an enum with a bunch of default items. PlotSceneWidget manages the
  * two default axes for you. If you want to add more axes, please have a look to the
  * ScaleItem::Id enum and use one of the predefined values or add others by yourself.
  *
  */
ScaleItem::ScaleItem(Orientation o, QGraphicsPlotItem *parent, ScaleItem::Id id)
    : QGraphicsObject(parent), PlotGeometryEventListener() {
    d = new ScaleItemPrivate();
    d->orientation = o;
    d->axisId = id;
    d->scaleLabelInterface = NULL;
    d->tickWidth = 10;
    d->axisLabelDist = d->tickWidth;
    d->maxlabelw = -1.0;
    d->labelHeight = 10;
    d->labelMargin = 5.0;
    d->fontSize = 10.0;
    d->axisTitleFont = d->font = QFont("FreeSans");

    d->axisTitleFont.setItalic(true);
    d->font.setPointSizeF(d->fontSize);
    /* Free Sans scales well */
    d->font.setFamily("FreeSans");
    plotAreaW = parent->boundingRect().width();
    plotAreaH = parent->boundingRect().height();
    d->actualTickStepLen = updateStepLen();
    if(o == Vertical) {
        d->axisTitleFont.setBold(true);
        d->axisTitleFont.setPointSizeF(d->axisTitleFont.pointSizeF() + 1);
        d->axisLabelRotation = 0.0;
        /* first draw Y axis, so that X axis labels are not covered by the Y axis
         * grid. Actually, X axis labels may be time scales and so it is ugly to
         * have them cut by the Y grid.
         */
        setZValue(-1);
    }
    d->mAxisTitleHeight = 0.0;
    d->mAxisTitleWidth = 0.0;
    /* if this axis is created after plot has been setup, then let the plot rect be
     * initialized from the current plot rect
     */
    d->plotRect = parent->boundingRect();
}

int ScaleItem::type() const {
    return ScaleItemType;
}

/** \brief Install an AxisChangeListener in order to be notified when axis bounds change
  *        or the auto scale property changes.
  *
  * @param l an implementation of the AxisChangeListener interface
  * @see AxisChangeListener
  */
void ScaleItem::installAxisChangeListener(AxisChangeListener *l)
{
    /* add new listener */
    d->axisChangeListeners.append(l);

    /* initialize each listener with the currently initialized data.
     * Actually, not all curves may be added at once; instead, curves
     * can be added at any point in time, and so they need be initialized
     * with the current canvas rect, bounds and autoscale properties
     */
    l->plotAreaChanged(d->plotArea);
    if(d->orientation == Horizontal)
        l->xAxisBoundsChanged(d->lowerBound, d->upperBound);
    else
        l->yAxisBoundsChanged(d->lowerBound, d->upperBound);
    /* listener (e.g. SceneCurve) will recalculate bounds if autoscale is enabled */
    l->axisAutoscaleChanged(d->orientation, d->autoScale);
    if(d->actualTickStepLen > -1)
        l->tickStepLenChanged(d->actualTickStepLen);
    if(!d->actualLabelsFormat.isEmpty())
        l->labelsFormatChanged(d->actualLabelsFormat);
}

/** \brief returns the label associated to the value passed as parameter, if a ScaleLabelInterface
  *        has been installed on the ScaleItem, the value itself otherwise.
  *
  * @param value the value on the ScaleItem whose label you want to retrieve
  * @return the label associated to the value value.
  *
  * @see installScaleLabelInterface
  * @see ScaleLabelInterface
  */
QString ScaleItem::label(double value) const
{
    if(d->scaleLabelInterface)
        return d->scaleLabelInterface->label(value);

    QString l;
    l = QString::asprintf(d->actualLabelsFormat.toStdString().c_str(), value);
    return l;
}

/** \brief Provide a custom label manager for the ScaleItem.
  *
  * If a ScaleLabelInterface implementation is provided, then the labels text
  * will be obtained by ScaleLabelInterface::label method.
  *
  * @see ScaleLabelInterface
  * @param iface an object implementing the ScaleLabelInterface interface
  *
  * The ScaleItem class does not take ownership of the scale label interface.
  * This means that removeScaleLabelInterface will not delete the installed interface
  * and that subsequent calls to installScaleLabelInterface will not delete previously
  * installed scale label interfaces.
  */
void ScaleItem::installScaleLabelInterface(ScaleLabelInterface *iface)
{
    d->scaleLabelInterface = iface;
    this->updateLabelsCache();
    update();
}

/** \brief removes the custom scale label interface implementation.
  *
  * If this method is called, then the ScaleItem labels text will simply display
  * the values of the x data.
  */
void ScaleItem::removeScaleLabelInterface() {
    d->scaleLabelInterface = NULL;
    update();
}

/** \brief returns the ScaleLabelInterface that was installed with installScaleLabelInterface,
  *         null otherwise.
  *
  * @return the installed ScaleLabelInterface or NULL if it was not installed.
  * @see installScaleLabelInterface
  *
  */
ScaleLabelInterface *ScaleItem::scaleLabelInterface() const {
    return d->scaleLabelInterface;
}

void ScaleItem::removeAxisChangeListener(AxisChangeListener *l) {
    d->axisChangeListeners.removeAll(l);
}

void ScaleItem::setOrientation(Orientation o)
{
    d->orientation = o;
}

ScaleItem::Orientation ScaleItem::orientation() const
{
    return d->orientation;
}

ScaleItem::Id ScaleItem::axisId() const
{
    return d->axisId;
}

/** \brief returns the upper bound (maximum value) of the scale
  *
  *
  * Axis auto scale must be disabled otherwise this call will have no effect.
  *
  * @return the upper bound of the ScaleItem
  *
  * @see setUpperBound
  * @see setLowerBound
  * @see lowerBound
  * @see setAxisAutoscaleEnabled
  */
double ScaleItem::upperBound() const {
    return d->upperBound;
}

/** \brief returns the lower bound (minimum value) of the scale
  *
  *
  * Axis auto scale must be disabled otherwise this call will have no effect.
  *
  * @return the lower bound of the ScaleItem
  *
  * @see setUpperBound
  * @see setLowerBound
  * @see upperBound
  * @see setAxisAutoscaleEnabled
  */
double ScaleItem::lowerBound() const {
    return d->lowerBound;
}


/** \brief sets upper and lower bounds
  *
  * Axis auto scale must be disabled otherwise this call will have no effect.
  *
  * @see setUpperBound
  * @see setLowerBound
  *
  * \note recalculates the actual tick step len if not set by the user.
  */
void ScaleItem::setBounds(double lowerBound, double upperBound)
{
    if(lowerBound < upperBound)
    {
        if(d->lowerBound != lowerBound || d->upperBound != upperBound)
        {
            d->lowerBound = lowerBound;
            d->upperBound = upperBound;

            static_cast<QGraphicsPlotItem *>(parentItem())->boundsChanged();

            /* needs to be called before updateLabelsFormat */
            updateStepLen();
            /* once the tick step len is up to date, update labels format */
            if(d->axisLabelsFormat.isEmpty()) /* avoid a function call if not needed */
                updateLabelsFormat(d->axisLabelsFormat);

            mNotifyBoundsChanged();

            /* labels width might have changed */
            updateLabelsCache();

            emit upperBoundChanged(upperBound);
            emit lowerBoundChanged(lowerBound);

            /* update */
            prepareGeometryChange();
            update();
//            d->autoScale = false;
        }
    }
    else
        perr("ScaleItem::setBounds: upper bound must be greater than lower bound!");
}

/** \brief sets the axis maximum value (upper bound)
  *
  * @param ub the value of the upper bound.
  *
  * axisAutoscaleEnabled property must be set to false.
  *
  * @see setLowerBound
  * @see upperBound
  * @see setAxisAutoscaleEnabled
  */
void ScaleItem::setUpperBound(double ub)
{
    if(ub > d->lowerBound)
    {
        d->upperBound = ub;
        static_cast<QGraphicsPlotItem *>(parentItem())->boundsChanged();

        /* needs to be called before mUpdateLabelsFormat */
        updateStepLen();

        /* once the tick step len is up to date, update labels format */
        if(d->axisLabelsFormat.isEmpty()) /* avoid a function call if not needed */
            updateLabelsFormat(d->axisLabelsFormat);

        mNotifyBoundsChanged();

        /* labels width might have changed */
        updateLabelsCache();

        emit upperBoundChanged(ub);

        prepareGeometryChange();

        update();

//        d->autoScale = false;
    }
    else
        perr("ScaleItem::setUpperBound: upper bound must be greater than lower bound (which is %f)",
             d->lowerBound);
}

/** \brief sets the axis minimum value (lower bound)
  *
  * @param lb the value of the lower bound.
  *
  * axisAutoscaleEnabled property must be set to false.
  *
  * @see setUpperBound
  * @see upperBound
  * @see lowerBound
  * @see setAxisAutoscaleEnabled
  */
void ScaleItem::setLowerBound(double lb)
{
    if(lb < d->upperBound)
    {
        d->lowerBound = lb;
        static_cast<QGraphicsPlotItem *>(parentItem())->boundsChanged();

        /* needs to be called before mUpdateLabelsFormat */
        updateStepLen();
        /* once the tick step len is up to date, update labels format */
        if(d->axisLabelsFormat.isEmpty()) /* avoid a function call if not needed */
            updateLabelsFormat(d->axisLabelsFormat);

        mNotifyBoundsChanged();

        /* labels width might have changed */
        updateLabelsCache();
        emit lowerBoundChanged(lb);
        prepareGeometryChange();
        update();
//        d->autoScale = false;
    }
    else
        perr("ScaleItem::setLowerBound: lower bound must be less than upper bound (which is %f)",
             d->upperBound);

}

double ScaleItem::xlbAutoscaleMargin() const {
    return d->xlbMargin;
}

/** \brief When in auto scale mode, the autoscaleMargin property defines a margin
 *  to apply to the span. Factor between 0 and 1
 */
void ScaleItem::setXlbAutoscaleMargin(double spanPercent) {
    d->xlbMargin = spanPercent;
}

double ScaleItem::xubAutoscaleMargin() const {
    return d->xubMargin;
}

void ScaleItem::setXubAutoscaleMargin(double m)
{
    d->xubMargin = m;
}

double ScaleItem::ylbAutoscaleMargin() const {
    return d->ylbMargin;
}

void ScaleItem::setYlbAutoscaleMargin(double m) {
    d->ylbMargin = m;
}

double ScaleItem::yubAutoscaleMargin() const {
    return d->yubMargin;
}

void ScaleItem::setYubAutoscaleMargin(double m) {
    d->yubMargin = m;
}

void ScaleItem::mNotifyBoundsChanged()
{
    foreach(AxisChangeListener* l, d->axisChangeListeners)
    {
        if(d->orientation == Horizontal)
            l->xAxisBoundsChanged(d->lowerBound, d->upperBound);
        else
            l->yAxisBoundsChanged(d->lowerBound, d->upperBound);
    }
}

/** \brief Sets the lower bound according to a given QDateTime timestamp.
 *
 * Sets the lower bound according to a given QDateTime timestamp.
 * @param t a QDateTime representing the lower bound of the scale.
 *
 * \note the QDateTime is converted into a double with the millisecond
 *       precision: t.toSecsSinceEpoch() + 0.001 * t.time().msec()
 *
 * \note This method is useful when your scale is a time scale.
 *
 * @see TimeScaleLabel
 *
 * @see setUpperBoundDateTime
 * @see lowerBoundDateTime
 *
 */
void ScaleItem::setLowerBoundDateTime(const QDateTime& t)
{
    setLowerBound(t.toSecsSinceEpoch() + 0.001 * t.time().msec());
}

/** \brief Sets the upper bound according to a given QDateTime timestamp.
 *
 * Sets the upper bound according to a given QDateTime timestamp.
 *
 * \note This method is useful when your scale is a time scale.
 *
 * @see setLowerBoundDateTime
 * @see lowerBoundDateTime
 *
 * @see TimeScaleLabel
 *
 */
void ScaleItem::setUpperBoundDateTime(const QDateTime& t)
{
    setUpperBound(t.toSecsSinceEpoch() + 0.001 * t.time().msec());
}

QDateTime ScaleItem::doubleToDateTime(double d) const
{
    double ms = d - floor(d);
    QDateTime t;
    t.setSecsSinceEpoch(floor(d));
    t = t.addMSecs(ms);
    return t;
}

/** \brief returns the lower bound as a QDateTime object
 *
 * @return the lower bound as a QDateTime object
 *
 * \par Conversion
 * The decimal part of the double value representing the axis bound
 * is used to set the milliseconds, the integer part is interpreted as
 * a unix timestamp, i.e. the number of seconds that have passed since
 * 1970-01-01T00:00:00, Coordinated Universal Time.
 *
 * @see setUpperBoundDateTime
 * @see setLowerBoundDateTime
 * @see upperBoundDateTime
 *
 * @see TimeScaleLabel
 */
QDateTime ScaleItem::lowerBoundDateTime() const {
    return doubleToDateTime(d->lowerBound);
}

/** \brief returns the upper bound as a QDateTime object
 *
 * @return the upper bound as a QDateTime object
 * @see lowerBoundDateTime
 */
QDateTime ScaleItem::upperBoundDateTime() const
{
    return doubleToDateTime(d->upperBound);
}

/** \brief returns the value of the axisAutoscaleEnabled property
  *
  * @return true the axis scale is set according to the maximum value
  *         of the curves attached to the axis itself.
  * @return false the axis range is set according to upperBound and lowerBound
  *         properties.
  *
  * @see setUpperBound
  * @see setLowerBound
  * @see upperBound
  * @see lowerBound
  * @see setAxisAutoscaleEnabled
  *
  */
bool ScaleItem::axisAutoscaleEnabled() const {
    return d->autoScale;
}

/** \brief enables or disables the axis auto scaling
  *
  *
  * @see setUpperBound
  * @see setLowerBound
  * @see upperBound
  * @see lowerBound
  * @see axisAutoscaleEnabled
  */
void ScaleItem::setAxisAutoscaleEnabled(bool en)
{
    d->autoScale = en;
    foreach(AxisChangeListener* l, d->axisChangeListeners)
        l->axisAutoscaleChanged(d->orientation, en);
    emit autoscaleEnabledChanged(en);
}

/** \brief defines the distance between the backbone and the start of the label's text
  *
  * @return the distance, in pixels, between the backbone of the axis and the start of text.
  *
  * The default value is 5
  *
  * @see setAxisLabelDist
  */
double ScaleItem::axisLabelDist() const {
    return d->axisLabelDist;
}

/** \brief sets the distance between the backbone and the start of the label's text.
  *
  * @param d the distance, in scene coordinates, between the backbone and the start of text
  *        of each label.
  */
void ScaleItem::setAxisLabelDist(double dv) {
    d->axisLabelDist = dv;
    update();
}

/** \brief Returns the canvas rect, which is the rect where the curves
  *        can be drawn.
  *
  * This rect does not coincide with the plot rect if the axis labels
  * do not fit into the canvas.
  */
QRectF ScaleItem::plotArea() const {
    return d->plotArea;
}

void ScaleItem::setTickStepLen(double sLen)
{
    d->tickStepLen = sLen;
    if(sLen != -1)
    {
        d->actualTickStepLen = sLen;
        foreach(AxisChangeListener* l, d->axisChangeListeners)
            l->tickStepLenChanged(sLen);
    }
    else
        d->actualTickStepLen = updateStepLen();

    /* ok, after d_ptr->actualTickStepLen is up to date */
    updateLabelsCache();

    prepareGeometryChange();
}

double ScaleItem::tickStepLen() const {
    return d->tickStepLen;
}

void ScaleItem::setAxisLabelsEnabled(bool en) {
    d->labelsEnabled = en;
    updateLabelsCache();
    prepareGeometryChange();
}

bool ScaleItem::axisLabelsEnabled() const
{
    return d->labelsEnabled;
}

bool ScaleItem::ticksEnabled() const {
    return d->ticksEnabled;
}

void ScaleItem::setAxisLabelsRotation(double angle)
{
    d->axisLabelRotation = angle;
    update();
}

double ScaleItem::axisLabelsRotation() const
{
    return d->axisLabelRotation;
}

void ScaleItem::setAxisLabelsFormat(const QString& fmt) {
    d->axisLabelsFormat = fmt;
    updateLabelsFormat(fmt);
    updateLabelsCache();
    prepareGeometryChange();
}

QString ScaleItem::axisLabelsFormat() const {
    return d->axisLabelsFormat;
}

void ScaleItem::setTicksEnabled(bool en) {
    d->ticksEnabled = en;
    update();
}

void ScaleItem::setGridEnabled(bool en) {
    d->gridEnabled = en;
    update();
}

bool ScaleItem::gridEnabled() const {
    return d->gridEnabled;
}

void ScaleItem::setGridColor(const QColor& c) {
    d->gridColor = c;
    update();
}

void ScaleItem::setAxisColor(const QColor& c) {
    d->axisColor = c;
    update();
}

QColor ScaleItem::axisColor() const {
    return d->axisColor;
}

QColor ScaleItem::gridColor() const {
    return d->gridColor;
}

void ScaleItem::setFont(const QFont& f) {
    d->font = f;
    updateLabelsCache();
    update();
}

QFont ScaleItem::font() const {
    return d->font;
}

QFont ScaleItem::axisTitleFont() const
{
    return d->axisTitleFont;
}

QColor ScaleItem::axisTitleColor() const
{
    return d->axisTitleColor;
}

QString ScaleItem::axisTitle() const
{
    return d->axisTitle;
}

void ScaleItem::fullVectorUpdate()
{

}

void ScaleItem::setAxisTitleFont(const QFont &fo) {
    d->axisTitleFont = fo;
    /* not to calculate font metrics at every refresh */
    mRecalculateAxisTitleSize();
}

void ScaleItem::setAxisTitleColor(const QColor & co) {
    d->axisTitleColor = co;
}

void ScaleItem::setAxisTitle(const QString & ti) {
    d->axisTitle = ti;
    mRecalculateAxisTitleSize();
}

double ScaleItem::maxLabelWidth() const
{
    return d->maxlabelw;
}

void ScaleItem::adjustScaleBounds(double newMin, double newMax)
{
    double min = d->lowerBound;
    double max = d->upperBound;

    if(newMin < min || d->minMaxUnset)
        min = newMin;
    if(newMax > max || d->minMaxUnset)
        max = newMax;
    d->minMaxUnset = false;
    if(d->lowerBound != min || d->upperBound != max)
        setBounds(min, max); /* updates tick step len and labels cache */
}

void ScaleItem::affectingBoundsPointsRemoved()
{
    if(d->autoScale && !static_cast<QGraphicsPlotItem *>(parentItem())->inZoom())
    {
        printf("\e[0;32maffectingBoundsPointsRemoved().....\e[0m\n");
        /* must obtain again all maximum and minimum values from each curve */
        setBoundsFromCurves();
        /* correct scales if necessary */
        //    adjustScaleBounds();
    }
}

void ScaleItem::plotRectChanged(const QRectF &newRect) {
    update();
}

/** \brief Empty body. We are not interested in area changes.
 *
 * The area passed as parameter takes into account the scaling factor
 * of the view (not needed here).
 */
void ScaleItem::plotAreaChanged(const QSizeF &)
{
    updateStepLen();
    updateLabelsCache();
}

/** \brief is called when the plot zoom level changes.
 *
 * This method is called when the plot zoom level changes.
 *
 * @param level the level of the zoom
 *
 * \note Together with axes bound change, it is the third case in which
 * you need to calculate the tick step length again.
 *
 */
void ScaleItem::plotZoomLevelChanged(int level)
{
    d->plotZoomLevel = level;
    updateStepLen();
    updateLabelsCache();
    prepareGeometryChange();
}

void ScaleItem::itemsAboutToBeDrawn()
{
    d->minMaxUnset = true;
}

int ScaleItem::mGetDecimals(double q)
{
    int decimals = 0;
    while(q < 1)
    {
        decimals++;
        q = q * 10;
    }
    return decimals;
}

void ScaleItem::mRecalculateAxisTitleSize()
{
    QFontMetrics fm(d->axisTitleFont);
    d->mAxisTitleHeight = fm.height();
    d->mAxisTitleWidth = fm.horizontalAdvance(d->axisTitle);
}

/* updates the actualLabelsFormat private variable.
 *
 * If the desiredFormat hasn't been specified, then the method
 * needs the actualTickStepLen to be previously updated.
 *
 */
void ScaleItem::updateLabelsFormat(const QString& desiredFormat)
{
    QString previousFormat = d->actualLabelsFormat;
    if(!desiredFormat.isEmpty())
        d->actualLabelsFormat = desiredFormat;
    else
    {
        double x1, x2;
        x1 = d->lowerBound;
        x2 = d->upperBound;

        if(x2 >= x1)
        {
            qreal q = d->actualTickStepLen;
            if(q > 1 || q == 0)
                d->actualLabelsFormat = "%.0f";
            else
                d->actualLabelsFormat = QString("%.%1f").arg(mGetDecimals(q));
        }
    }
    /* notify axis label format has changed */
    if(previousFormat != d->actualLabelsFormat)
    {
        foreach(AxisChangeListener *l, d->axisChangeListeners)
            l->labelsFormatChanged(d->actualLabelsFormat);
    }
}

/* This method rebuilds the labels cache hash.
 * This method assumes that d)ptr->actualTickStepLen is up to date.
 * Call updateStepLen before if necessary
 *
 */
void ScaleItem::updateLabelsCache()
{
    double x, x0 = 0;
    double max = 0.0;
    double width;
    double x1 = d->lowerBound;
    double x2 = d->upperBound;
    /* assume that d_ptr->actualTickStepLen is up to date */
    double tickDist = d->actualTickStepLen;
    double originPercent;
    bool ok;
    QPair<double, double> originPosPercent;

    QString textLabel;
    QFontMetrics fm(d->font);
    /* clear the labels cache */
    d->labelsCacheHash.clear();

    ScaleItem *associatedAxis = static_cast<QGraphicsPlotItem *>(parentItem())->associatedAxis(this->axisId());
    if(associatedAxis)
        originPosPercent = static_cast<QGraphicsPlotItem *>(parentItem())->associatedOriginPosPercentage(associatedAxis->axisId(), d->axisId, &ok);

    if(d->orientation == Horizontal)
        originPercent = originPosPercent.first;
    else
        originPercent = originPosPercent.second;

    if(associatedAxis && ok)
        x0 = x1 + (x2 - x1) * originPercent;

    x = x0;
    while(x <= x2)
    {
        if(d->scaleLabelInterface)
            textLabel = d->scaleLabelInterface->label(x);
        else /* no, just return the number */
            textLabel = QString::asprintf(qstoc(d->actualLabelsFormat), x);
        /* add item to cache */
        d->labelsCacheHash.insert(x, textLabel);
        width = fm.horizontalAdvance(textLabel);
        if(max < width)
        {
            max = width;
            d->longestLabel = textLabel;
        }
        x += tickDist;
    }

    x = x0 - tickDist;
    while(x >= x1)
    {
        if(d->scaleLabelInterface)
            textLabel = d->scaleLabelInterface->label(x);
        else /* no, just return the number */
            textLabel = textLabel.asprintf(qstoc(d->actualLabelsFormat), x);
        /* add item to cache */
        d->labelsCacheHash.insert(x, textLabel);
        width = fm.horizontalAdvance(textLabel);
        if(max < width)
        {
            max = width;
            d->longestLabel = textLabel;
        }
        x -= tickDist;
    }
    d->maxlabelw = max + 1;
}

void ScaleItem::setVisible(bool visible) {
//    if(!visible) {
//        d->plotArea = d->plotRect = static_cast<QGraphicsPlotItem *>(parentItem())->boundingRect();
//    }
    d->enabled = visible;
    update();
}

/* recalculates the step len.
 * This method needs to be called when upper or lower bound changes or
 * when the plot zoom level changes.
 */
double ScaleItem::updateStepLen()
{
    double sLen;
    if(d->tickStepLen != -1) {
        sLen = d->tickStepLen;
    }
    else
    {
        double span = d->upperBound - d->lowerBound;
        double scale = 1.0, factor, x;
        span /= scale;
        double magnitude = floor(log10(span));
        factor = span / pow(10,magnitude);

        if(factor  < 2.5)
            x = 1;
        else if(factor < 5)
            x = 2;
        else
            x = 5;

        sLen = x * pow(10, (magnitude - 1));
    }

    if(d->actualTickStepLen != sLen) {
        d->actualTickStepLen = sLen;
        foreach(AxisChangeListener* l, d->axisChangeListeners) {
            l->tickStepLenChanged(sLen);
        }
    }
    return sLen;
}

// returns true if bounds have changed
bool ScaleItem::setBoundsFromCurves()
{
    d->minMaxUnset = true;
    QList<SceneCurve *> curves = static_cast<QGraphicsPlotItem *>(parentItem())->curvesForAxes(d->axisId, d->orientation);
    if(!curves.size())
        return false;
    int i;
    double min = 0.0, max = 0.0, span;
    SceneCurve *c;
    Data *da ;
    unsigned int visibleCurvesCnt = 0;
    switch(d->orientation)
    {
    case Horizontal:
        for(i = 0; i < curves.size(); i++)
        {
            c = curves[i];
            if(c->curveItem() && c->curveItem()->isVisible())
            {
                da = c->data();
                if(visibleCurvesCnt == 0 || da->xMin < min)
                    min = da->xMin;
                if(visibleCurvesCnt == 0 || da->xMax > max)
                    max = da->xMax;
                visibleCurvesCnt++;
            }
        }
        break;
    default:
        for(i = 0; i < curves.size(); i++)
        {
            c = curves[i];
            if(c->curveItem() && c->curveItem()->isVisible())
            {
                da = c->data();
                if(visibleCurvesCnt == 0 || da->yMin < min)
                    min = da->yMin;
                if(visibleCurvesCnt == 0 || da->yMax > max)
                    max = da->yMax;
                visibleCurvesCnt++;
            }
        }
        break;
    }
    if(max >= min)
    {
        if(max == min) /* only one value in the curve */
        {
            if(max != 0)
            {
                max += min * 0.03;
                min -= min * 0.04;
            }
            else
            {
                max = 1;
                min = -1;
            }
        }
        if(d->lowerBound != min || d->upperBound != max)
        {
            double lbmargin = d->orientation == ScaleItem::Horizontal ? d->xlbMargin : d->ylbMargin;
            double ubmargin = d->orientation == ScaleItem::Horizontal ? d->xubMargin : d->yubMargin;
            /* apply scale span adjustment (default is 2 % ) */
            span = max - min;
            if(d->lowerBound != min) { // min value can expand to the left
                min -= span * lbmargin;
                d->lowerBound = min;
            }
            if(d->upperBound != max) {
                max += span * ubmargin;
                d->upperBound = max;
            }
            /* needs to be called before mUpdateLabelsFormat */
            updateStepLen();
            /* once the tick step len is up to date, update labels format */
            if(d->axisLabelsFormat.isEmpty()) /* avoid a function call if not needed */
                updateLabelsFormat(d->axisLabelsFormat);

            updateLabelsCache();

            foreach(AxisChangeListener* l, d->axisChangeListeners)
            {
                if(d->orientation == Horizontal)
                    l->xAxisBoundsChanged(min, max);
                else
                    l->yAxisBoundsChanged(min, max);
            }
            emit upperBoundChanged(max);
            emit lowerBoundChanged(min);
            return true;
        }
    }
    else
        perr("ScaleItem::setBoundsFromCurves: max %f < min %f", max, min);
    return false;
}

void ScaleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *  )
{
    QPen axisPen(d->axisColor), gridPen(d->gridColor);
    axisPen.setWidthF(0.0);
    gridPen.setWidthF(0.0);
    QGraphicsPlotItem *plot_i = static_cast<QGraphicsPlotItem *>(parentItem());
    //    painter->setClipRect(option->exposedRect.toRect());
    double x1, x2, y1, y2, x, y, x0, y0;
    qreal px,  py, px0, py0;
    /* initialize plot area, width and height to plot item bounding rect */
    QRectF plota = plot_i->boundingRect();
    qreal rLeft = plota.left();
    qreal rTop = plota.top();
    qreal rRight = rLeft + plota.width();
    qreal rBottom = rTop + plota.height();

    qreal labelp;
    qreal labelHeight;
    qreal xMaxLabelSpace = 0.0, yMaxLabelSpace = 0.0;
    qreal tickStepLen;
    QString textLabel;
    QRectF txtRect(0, 0, 0, 0);
    painter->setFont(d->font);
    QFontMetrics fm(painter->font());
    labelHeight = fm.height();

    bool ok;
    QPair<double, double> o_pos;
    ScaleItem *associatedAxis = plot_i->associatedAxis(this->axisId());
    if(associatedAxis)
        o_pos = plot_i->associatedOriginPosPercentage(associatedAxis->axisId(), d->axisId, &ok);

    if(!associatedAxis || !ok)
    {
        perr("ScaleItem::paint: no other axis is associated to \"%s\" (id %d)"
             " or origin undefined between the current and associated axis", qstoc(objectName()),
             axisId());
        return;
    }

    tickStepLen = d->actualTickStepLen;
    float maxLabelW = (d->labelsEnabled ? d->maxlabelw : 0.0);
    float tickW = (d->ticksEnabled ? d->tickWidth : 0.0);
    float labelMargin = d->labelsEnabled ? d->labelMargin : 0.0;
    float axisLabelDist = d->labelsEnabled ? d->axisLabelDist : 0.0;
    /* if actual x and/or y labels format is not initialized, calculate it */
    if(d->actualLabelsFormat.isEmpty())
        updateLabelsFormat(d->axisLabelsFormat);

    switch(d->orientation)
    {
    case ScaleItem::Horizontal:
        x1 = d->lowerBound;
        x2 = d->upperBound;
        y1 = associatedAxis->lowerBound();
        y2 = associatedAxis->upperBound();

        xMaxLabelSpace = maxLabelW + tickW + labelMargin;
        yMaxLabelSpace = d->labelsEnabled ? (associatedAxis->maxLabelWidth() + tickW / 2.0) : 0.0;
        break;

    default:
        x1 = associatedAxis->lowerBound();
        x2 = associatedAxis->upperBound();
        y1 = d->lowerBound;
        y2 = d->upperBound;/* x tick len: if set to -1 then automatically choose 20 steps */

        xMaxLabelSpace = d->labelsEnabled ? associatedAxis->maxLabelWidth()+ tickW + labelMargin : 0.0;
        yMaxLabelSpace = maxLabelW + tickW / 2.0;
        break;
    }
    if(x1 == x2 || y1 == y2)
        return;

    x0 = x1 + (x2 - x1) * o_pos.first;
    y0 = y1 + (y2 - y1) * o_pos.second;

//    px0 = (plota.width() - 1) * (x0 - x1) / (x2 - x1) + rLeft;
//    py0 = plota.height() - 1 - ((plota.height() - 1) * (y0 - y1) / (y2 - y1) + rTop);

    px0 = plota.width() * (x0 - x1) / (x2 - x1) + rLeft;
    py0 = plota.height() - (plota.height()  * (y0 - y1) / (y2 - y1) + rTop);

    if(d->enabled) {
        if(px0 - yMaxLabelSpace < rLeft ||
                ((d->orientation == Vertical && o_pos.first == 0)
                 || (d->orientation == Horizontal && o_pos.second == 0))) {
            rLeft = yMaxLabelSpace;
            plota.setLeft(yMaxLabelSpace);
        }
        if(py0 + xMaxLabelSpace > rBottom ||
                ( (d->orientation == Horizontal && o_pos.first == 0)
                  || (d->orientation == Vertical && o_pos.second == 0) ) )
        {
            plota.setBottom(plota.bottom() - xMaxLabelSpace);
            rBottom = plota.bottom();
        }
    } // axis_enabled
    /* if axis labels have required a change in the space reserved to the curves
     * (canvas rect), then the canvas rect must be recalculated.
     * Let's use the inverted transform to calculate it.
     */
    if(d->plotArea != plota)  {
        foreach(AxisChangeListener* l, d->axisChangeListeners)
            l->plotAreaChanged(plota);
        d->plotArea = plota;
    }

    plotAreaW = plota.width();
    plotAreaH = plota.height();

    switch(d->orientation)
    {
    case ScaleItem::Horizontal:
//        py0 = plota.height() - 1 - ((plota.height() - 1) * (y0 - y1) / (y2 - y1) + rTop);
        py0 = plota.height() - ((plota.height()) * (y0 - y1) / (y2 - y1) + rTop);
        labelp = o_pos.first == 0 ? (rBottom - py0 + tickW/2.0 + labelMargin) : axisLabelDist;
        painter->setPen(d->enabled ? axisPen : gridPen);
        painter->drawLine(rLeft, py0 , rRight, py0);
        /* draw ticks starting from origin */
        d->mLastTickPos = (plota.height() - 1) * (x0 - x1) / (x2 - x1) + rLeft;

        x = x0;
        while(x <= x2)
        {
            px = (plota.width() - 1) * (x - x1) / (x2 - x1) + rLeft;
            if(d->gridEnabled && x != x0 && d->enabled) {/* x != x0 not to draw grid over axes */
                painter->setPen(gridPen);
                painter->drawLine(px, rTop, px, rBottom);
            }
            if(d->labelsEnabled && xMaxLabelSpace > 0) {
                /* ScaleLabelInterface installed ? */
                textLabel = d->labelsCacheHash.value(x);
                if((px - fm.height() > d->mLastTickPos || x == x0) &&
                        px + fm.height() / 2.0 <= plot_i->boundingRect().width())
                {
                    if(d->enabled) {
                        painter->setPen(axisPen);
                        txtRect.setRect(0, 0, maxLabelW, labelHeight);
                        painter->translate(px + labelHeight/2, labelp + py0);
                        painter->rotate(d->axisLabelRotation);
                        painter->drawText(txtRect, textLabel);
                        painter->rotate(-d->axisLabelRotation);
                        painter->translate(-px - labelHeight/2, -labelp - py0);
                    }
                    d->mLastTickPos = px;
                }
            }
            if(d->enabled && d->ticksEnabled) {
                painter->setPen(axisPen);
                painter->drawLine(px, py0 - tickW/2.0, px, py0 + tickW/2.0);
            }
            x = x + tickStepLen;
        }
        //        printf("\e[0m\n");

        /* draw ticks from origin backwards */
        d->mLastTickPos = (plota.width() - 1) * (x0 - x1) / (x2 - x1) + rLeft;
        x = x0 - tickStepLen;
        while( x >= x1)
        {
            px = (plota.width() - 1) * (x - x1)/ (x2 - x1) + rLeft;
            /* 1. draw grid */
            if(d->gridEnabled  && d->enabled) {
                painter->setPen(gridPen);
                painter->drawLine(px, rTop, px, rBottom);
            }

            /* 2. draw labels */
            if(d->labelsEnabled && xMaxLabelSpace > 0)
            {
                textLabel = d->labelsCacheHash.value(x);
                if(px + fm.height() < d->mLastTickPos && px + fm.height() / 2.0 <= plot_i->boundingRect().width())
                {
                    if(d->enabled) {
                        painter->setPen(axisPen);
                        txtRect.setRect(0, 0, fm.horizontalAdvance(textLabel), labelHeight);
                        painter->translate(px + labelHeight/2.0, py0 + labelp);
                        painter->rotate(d->axisLabelRotation);
                        painter->drawText(txtRect, textLabel);
                        painter->rotate(-d->axisLabelRotation);
                        painter->translate(-px - labelHeight/2.0, -py0 - labelp);
                    }
                    d->mLastTickPos = px;
                }
            }
            /* 3. draw ticks */
            if(d->enabled && d->ticksEnabled) {
                painter->setPen(axisPen);
                painter->drawLine(px, py0 -tickW/2.0, px, py0 + tickW/2.0);
            }

            x = x - tickStepLen;
        }

        /* Draw axis title */
        if(!d->axisTitle.isEmpty() && d->enabled)
        {
            painter->setFont(d->axisTitleFont);
            painter->setPen(QPen(d->axisTitleColor));
            painter->drawText(rRight - d->mAxisTitleWidth - 4, py0 - tickW/2 - 1, d->axisTitle);
        }
        //        printf("\e[0m\n");

//        painter->setPen(Qt::green);
//          painter->drawRect(boundingRect());

        break;

    case Vertical:
    default:
//        px0 = (plota.width() - 1) * (x0 - x1) / (x2 - x1) + rLeft;
        px0 = plota.width() * (x0 - x1) / (x2 - x1) + rLeft;
        labelp = o_pos.second == 0 ? (-px0 - maxLabelW +rLeft - tickW/2.0) : -axisLabelDist - maxLabelW;

        if(d->enabled) {
            painter->setPen(axisPen);
            painter->drawLine(px0, rTop , px0, rBottom);
        }

//        d->mLastTickPos = plota.height() - 1 - ((plota.height() - 1) * (y0 - y1) / (y2 - y1) + rTop);
        d->mLastTickPos = plota.height() -(plota.height() * (y0 - y1) / (y2 - y1) + rTop);

        /* draw ticks starting from origin */
        y = y0;
        while(y <= y2)
        {
//            py = (plota.height() - 1) - ((plota.height() - 1) * (y - y1) / (y2 - y1) + rTop);
            py = plota.height()  - (plota.height() * (y - y1) / (y2 - y1) + rTop);

            if(d->gridEnabled && y != y0 && d->enabled) /* y != y0 not to draw grid over axes */
            {
                painter->setPen(gridPen);
                painter->drawLine(rLeft, py, rRight, py);
            }

            if(d->enabled && d->ticksEnabled) {
                painter->setPen(axisPen);
                painter->drawLine(px0 - tickW/2.0, py, px0 + tickW/2.0, py);
            }

            if(d->labelsEnabled && xMaxLabelSpace > 0)
            {
                textLabel = d->labelsCacheHash.value(y);
                txtRect.setRect(labelp, -labelHeight/2, maxLabelW, fm.height());

                /* less than when  y positive, due to inverted Qt coordinate system for y axis */
                if((py + fm.height() < d->mLastTickPos && py - fm.height()/2.0 >= 0) || y == y0)
                {
                    if(d->enabled) {
                        painter->translate(px0, py);
                        painter->rotate(d->axisLabelRotation);
                        painter->drawText(txtRect, textLabel);
                        painter->rotate(-d->axisLabelRotation);
                        painter->translate(-px0, -py);
                    }
                    d->mLastTickPos = py;
                }
            }
            y = y + tickStepLen;
        }

        d->mLastTickPos = plota.height() - 1 - ((plota.height() - 1) * (y0 - y1) / (y2 - y1) + rTop);
        /* draw ticks from origin backwards */
        y = y0 - tickStepLen;
        while(y >= y1)  {
            /* y axis direction is inverted with respect to Qt coordinates system.
             * So sceneR.height() - ....
             */
//            py = plota.height() - 1 - ((plota.height() - 1) * (y - y1)/ (y2 - y1) + rTop);
           py = plota.height()  - ((plota.height() ) * (y - y1)/ (y2 - y1) + rTop);

            if(d->gridEnabled && d->enabled)   {
                painter->setPen(gridPen);
                painter->drawLine(rLeft, py, rRight, py);
            }

            if(d->enabled  && d->ticksEnabled) {
                painter->setPen(axisPen);
                painter->drawLine(px0 - tickW/2.0, py, px0 + tickW/2.0, py);
            }

            if(d->labelsEnabled && xMaxLabelSpace > 0) {
                textLabel = d->labelsCacheHash.value(y);
                txtRect.setRect(labelp, -labelHeight/2, fm.horizontalAdvance(textLabel), labelHeight);
                /* > due to inverted Qt coordinate system for y axis
                 */
                if(py - labelHeight > d->mLastTickPos && py - fm.height() / 2.0 > 0) {
                    if(d->enabled) {
                        painter->translate(px0, py);
                        painter->rotate(d->axisLabelRotation);
                        painter->drawText(txtRect, textLabel);
                        painter->rotate(-d->axisLabelRotation);
                        painter->translate(-px0, -py);
                    }
                    d->mLastTickPos = py;
                }
            }
            y = y - tickStepLen;
        }

        /* Draw axis title */
        if(!d->axisTitle.isEmpty() && d->enabled) {
            painter->setFont(d->axisTitleFont);
            QPen axisTitlePen(d->axisTitleColor);
            axisTitlePen.setWidthF(0.0);
            painter->setPen(axisTitlePen);
            painter->drawText(px0 + tickW/2.0 + 3, rTop + d->mAxisTitleHeight + 2,  d->axisTitle);
        }
        //        painter->setPen(Qt::blue);
        //                QPen p = painter->pen();
        //                QColor c(Qt::blue);
        //        c.setAlpha(80);
        //                p.setColor(c);
        //        p.setWidth(2);
        //                painter->setPen(p);
        //                painter->drawRect(rect);
        ////        painter->drawRect(boundingRect());

        break;

        /* draw the scene rect for debug purposes */
        // painter->drawRect(-pos().x(), -pos().y(), sceneR.width(), sceneR.height());
    }

    /// DEBUG STUFF
    ///

    //    QPen canvasPen(Qt::green);
    //    canvasPen.setWidthF(1.1);
    //    painter->setPen(canvasPen);
    //    painter->drawRect(d->plotArea);
    //    //    painter->setPen(Qt::blue);
    //    //    painter->drawRect(/*tran.mapRect*/(d->canvasRect));
    //    QPen pp(Qt::black);
    //    pp.setStyle(Qt::DashLine);
    //    painter->setPen(pp);
    //    painter->drawRect(plot_i->boundingRect());
    //    qDebug() << __PRETTY_FUNCTION__ << "canvas rect" << d->plotArea << "rect" << plotarea;

}

QRectF ScaleItem::boundingRect () const {
    return static_cast<QGraphicsPlotItem *>(parentItem())->boundingRect();
}


