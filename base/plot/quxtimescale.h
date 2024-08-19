#ifndef QUXTIMESCALE_H
#define QUXTIMESCALE_H

#include <qnamespace.h> // Qt::TimeSpec
#include <qwt_plot.h>

class QwtPlot;

/*!
 * \brief Install a time scale on the given plot and axis.
 *
 * Default axis if not specified: *x bottom*
 * Optionally show date information alongside time and set a custom time spec.
 */
class QuXTimeScale {
public:
    QuXTimeScale(QwtPlot* plot, int axis_pos = QwtPlot::xBottom,
        bool show_date = false, Qt::TimeSpec timeSpec= Qt::LocalTime);
};

#endif // QUXTIMESCALE_H
