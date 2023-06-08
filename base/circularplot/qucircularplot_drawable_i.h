#ifndef QUCIRCULARPLOT_DRAWABLE_I_H
#define QUCIRCULARPLOT_DRAWABLE_I_H

class QPainter;
class QRectF;
class QWidget;
class QuCircularPlotEngine;

class QuCircularPlotDrawable_I {
public:

    /*!
     * \brief returns true if the drawable scales with zooming
     *
     * \return true the drawable scales according to the zoom. It will be painted with the painter *transform* applied
     *         false the drawable does not scale with the zoom. It will be painted after the painter is *restored* or
     *         before the transformation is applied, according to the z value
     * \see z
     */
    virtual bool scales() = 0;

    /*!
     * \brief z defines the z order of the element that is drawn
     * \return the z order
     *
     * Drawable elements with a lower z value are drawn before.
     *
     * \par Important note
     * Axes, curves, baseline and bounds are assumed to have z equal to 0.
     * Drawables with z less than or *equal to* 0 will be drawn *before* the axes and the curves. Drawables with z greater than
     * zero will be drawn *after* axes, bounds and curves.
     */
    virtual int z() const = 0;

    /*!
     * \brief paint the drawable on the provided painter.
     *
     * Draw a custom shape on the painter.
     * The plot engine can be used to get the plot configuration
     *
     * \par Note
     * Inner and outer radiuses correspond to the *axes bounds*.
     * The *valid* area for curves is defined between the inner_radius and the outer_radius.
     * Outside this section, curves may be drawn deformed.
     *
     * Drawables can paint anywhere.
     *
     * \param p the active painter
     * \param plot_e the plot engine
     * \param inner_radius start of valid scale section
     * \param outer_radius end of valid scale section
     * \param rect the drawing area, directly from QGraphicsItem::paint
     * \param widget pointer to QWidget, directly from QGraphicsItem::paint
     */
    virtual void draw(QPainter *p,
                      const QuCircularPlotEngine *plot_e,
                      double inner_radius,
                      double outer_radius,
                      const QRectF &rect,
                      QWidget *widget) = 0;
};

#endif // QUCIRCULARPLOT_DRAWABLE_I_H
