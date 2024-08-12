#ifndef QUXTIMESCALE_H
#define QUXTIMESCALE_H

#include <qwt_axis.h>
#include <qnamespace.h> // Qt::TimeSpec

class QwtPlot;

/*!
 * \brief Install a time scale on the given plot and axis.
 *
 * Default axis if not specified: *x bottom*
 * Optionally show date information alongside time and set a custom time spec.
 */
class QuXTimeScale {
public:
    QuXTimeScale(QwtPlot* plot, int axis_pos = QwtAxis::XBottom,
        bool show_date = false, Qt::TimeSpec timeSpec= Qt::LocalTime);
};

#endif // QUXTIMESCALE_H
