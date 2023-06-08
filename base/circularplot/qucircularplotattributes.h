#ifndef QUCIRCULARPLOTATTRIBUTES_H
#define QUCIRCULARPLOTATTRIBUTES_H

#include <QColor>

class OOBDistort;

/*!
 * \brief Holds QuCircularPlot options
 *
 * This struct hierarchically stores the characteristics of the plot
 *
 * \section axes
 * Defines the axes configuration
 *
 * \subsection x
 * Defines the x axis configuration
 * - baseline
 * - autoscale
 * - lower bound
 * - upper bound
 *
 * \subsection y
 * Defines the y axis configuration:
 * - scale_inverted
 * - autoscale
 * - lower bound
 * - upper bound
 *
 * \section geom
 * Defines the geometrical characteristics of the plot: the inner and outer radius representing the
 * axis limits (lower and upper) and the kind of distortion that shall be applied to curves when
 * their points exceed the (non automatic) Y bounds.
 *
 * \subsection selection
 * Geometry for the selection (movable selection area and selected point on the curve)
 *
 * \section paint
 * Contains drawing options (show hide elements) and *colors* in the dedicated section
 *
 * \subsection colors
 * Define colors for several elements
 *
 * \section Note
 * Whenever you change one or more attributes, you may want to trigger an immediate *update*
 * on the plot (either item or widget)
 *
 * \section Example
 *
 * \code
    QuCircularPlotI *ni = new QuCircularPlotI(nullptr, QSize(300, 300));
    QuCircularPlotEngine *pie = ni->engine();
    qucircularplot_attributes &a = pie->a;
    a.axes.y.autoscale = true;  // enable y axis autoscale
    a.geom.inner_radius_factor = 0.1; // widen inter axes area
    a.paint.colors.background = QColor(Qt::white);  // light color scheme
    ni->update();
 * \endcode
 *
 */
struct qucircularplot_attributes
{
    struct {
        struct {
            float baseline = 0.0f;
            bool autoscale = true;
            double lb = -1000;
            double ub = 1000;  // "axis" bounds, either manual or auto
        } x;

        struct {
            bool autoscale = true;
            bool scale_inverted = false;
            double lb = -1000;
            double ub = 1000;
        } y;

    } axes;

    struct {

        double inner_radius_factor = 0.2; //!< where (geometrically) the lower bound is
        double outer_radius_factor = 0.8; //!< where (geometrically) the upper bound is


        OOBDistort *oob_distortion = nullptr; //!< "out of bounds" curve distortion
                                    //!< defines how curves are distorted when points
                                    //!< exceed Y axis upper and lower bounds
                                    //!< Default must be initialized (QuCircularPlotEngine)

        struct {
            float aradius = 3.5; //!< radius of the (moving) circular selection area
            float yp_radius = 1.2; //!< radius of the point selected on the curve
        } selection; //!< selection attributes

    } geom; //!< geometry attributes: inner and outer radius, radius factor

    struct {
        bool show_values = false; //!<  show values on the plot. Default: false. OK when there's little data
        bool show_points = false; //!<  show data points (default: false, while show_curves is true by default)
        bool show_bounds = true; //!<  draw one circle on the lower bound and one on the upper (default: yes)

        bool show_curves = true;  //!< show the curves (lines connecting the data points): default true
        bool show_xax = true; //!<  show the "X" axis: default: yes on the baseline
        bool show_origin = true; //!< show the "origin" at 9 o'clock default: true

        int y_axes = 0; //!< number of "Y" axes to show default: 0

        struct {

            QColor background = QColor(Qt::white);   //!<  background color
            QColor axes = QColor(Qt::lightGray);  //!<  axes color
        } colors;

    } paint; //!< "paint" attributes: show or hide things
};

#endif // QUCIRCULARPLOTATTRIBUTES_H
