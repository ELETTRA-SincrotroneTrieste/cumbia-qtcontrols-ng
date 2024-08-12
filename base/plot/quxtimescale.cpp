#include "quxtimescale.h"
#include <qwt_date_scale_draw.h>
#include <qwt_plot.h>

/*!
 * \brief Install the time scale draw on the given plot, and apply a -50 degrees rotation
 *        to the labels, aligning them using the flags Qt::AlignLeft | Qt::AlignBottom
 *
 * The applied rotation reduces the space needed for the X axis labels
 *
 * \param plot the plot
 * \param axis_pos QwtAxisId position, default: XBottom
 * \param show_date if true shows the date in addition to the time
 * \param timeSpec time spec, default: Qt::LocalTime
 *
 * To access the scale later, if *plot* is your plot and *axisId* the QwtAxisId:
 *
 * \code
 * QwtDateScaleDraw *sd = static_cast<QwtDateScaleDraw *>(plot->axisScaleDraw(axisId));
 * \endcode
 *
 * \see QuScalarPlot
 */
QuXTimeScale::QuXTimeScale(QwtPlot *plot,
                           int axis_pos,
                           bool show_date,
                           Qt::TimeSpec timeSpec)
{
    QwtDateScaleDraw *s = new QwtDateScaleDraw(timeSpec);
    if(show_date) {
        s->setDateFormat( QwtDate::Millisecond, "MMM.dd hh:mm:ss:zzz" );
        s->setDateFormat( QwtDate::Second, "MMM.dd hh:mm:ss" );
        s->setDateFormat( QwtDate::Minute, "MMM.dd hh:mm" );
        s->setDateFormat( QwtDate::Hour, "MMM.dd hh:mm" );
        s->setDateFormat( QwtDate::Day, "ddd dd MMM" );
        s->setDateFormat( QwtDate::Week, "Www" );
        s->setDateFormat( QwtDate::Month, "MMM" );
    }
    else {
        s->setDateFormat( QwtDate::Millisecond, "hh:mm:ss:zzz" );
        s->setDateFormat( QwtDate::Second, "hh:mm:ss" );
        s->setDateFormat( QwtDate::Minute, "hh:mm" );
        s->setDateFormat( QwtDate::Hour, "hh:mm" );
        s->setDateFormat( QwtDate::Day, "ddd dd MMM" );
        s->setDateFormat( QwtDate::Week, "Www" );
        s->setDateFormat( QwtDate::Month, "MMM" );
    }
    plot->setAxisScaleDraw(axis_pos, s);
    plot->setAxisLabelRotation(axis_pos, -50);
    plot->setAxisLabelAlignment(axis_pos, Qt::AlignLeft | Qt::AlignBottom );
}

