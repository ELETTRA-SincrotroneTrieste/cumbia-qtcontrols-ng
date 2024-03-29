#ifndef VERTICALSCALEWIDGET_H
#define VERTICALSCALEWIDGET_H

#include <externalscalewidget.h>
#include <QtUiPlugin/QDesignerExportWidget>

/** \brief Convenience class that creates a vertical scale.
 *
 * Read the ExternalScaleWidget class documentation for full details.
 *
 */
class QDESIGNER_WIDGET_EXPORT VerticalScaleWidget : public ExternalScaleWidget
{
    Q_OBJECT
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double tickStepLen READ tickStepLen WRITE setTickStepLen)
    Q_PROPERTY(double originPercentage READ originPercentage WRITE setOriginPercentage)
    Q_PROPERTY(double labelRotation READ labelRotation WRITE setLabelRotation)
    Q_PROPERTY(int tickLen READ tickLen WRITE setTickLen)
    Q_PROPERTY(int labelDistFromTick READ labelDistFromTick WRITE setLabelDistFromTick)
    Q_PROPERTY(bool isInverted READ isInverted WRITE setInverted)
    Q_PROPERTY(bool horizontal READ horizontal WRITE setHorizontal)
    Q_PROPERTY(bool tickDrawingInverted READ tickDrawingInverted WRITE setTickDrawingInverted)
    Q_PROPERTY(QString format READ format WRITE setFormat)
    Q_PROPERTY(bool syncAxisTicksWithPlot READ syncAxisTicksWithPlot WRITE setSyncAxisTicksWithPlot)
    Q_PROPERTY(bool syncAxisLabelsWithPlot READ syncAxisLabelsWithPlot WRITE setSyncAxisLabelsWithPlot)
public:

    /** \brief Creates an ExternalScaleWidget initialized to be a vertical scale.
     *
     * @see ExternalScaleWidget
     * @see HorizontalScaleWidget
     */
    VerticalScaleWidget(QWidget* parent);
};

#endif // VERTICALSCALEWIDGET_H
