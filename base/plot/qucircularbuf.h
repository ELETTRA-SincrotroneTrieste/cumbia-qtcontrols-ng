#ifndef QUCIRCULARBUF_H
#define QUCIRCULARBUF_H

#include <qubufbase.h>
#include <qwt_series_data.h>

class QuPlotDataBufP;


/*!
 * \brief A circular buffer, used by QuScalarPlot, that can track minimum and maximum values
 *        for the *Y* component, while it assumes that, in case custom *X* data is provided,
 *        it is already ordered ascending.
 *
 * A circular buffer with a fixed size that, if unspecified in the constructor, is initialized
 * to a number of points equal to one day data sampled at one second.
 * Data is added with one of the two available flavours of *append*:
 * \li one for the *Y* data only: in that case the *X* data is assumed to be of size *Y.size*
 * and filled with values from 0 to Y.size() - 1.
 * \li one for both *X* and *Y*
 *
 * \par Accessing elements
 * To correctly access values from the circular buffer, use one of QuCircularBuf::p,
 * QuCircularBuf::px or QuCircularBuf::py, to fetch the *i-th* point, *x* or *y* values
 * at coordinate *i*, respectively.
 *
 * The operator *[]* is equivalent to QuCircularBuf::p
 *
 * To get the actual *index* within the circular buffer, use the method
 * QuCircularBuf::index, that maps the *logical* (the user's) index to the one internal to
 * the buffer. After the buffer has been filled, mapping is required.
 *
 * QuCircularBuf::x and QuCircularBuf::y are public and available for direct access, keeping
 * in mind the mentioned index mapping.
 *
 * \par bounds tracking
 *
 * QuCircularBuf can be constructed with an option to track *y* minimum and maximum values
 * at each *append* operation. If it is constructed with also the *x* auto tracking, then
 * the *x lower bound is always the first element*, but the *x upper bound* is calculated
 * so as to be *a little higher* than the strictly x[x.size() - 1] value, in order to reduce
 * automatic X axis rescale operations, and above all full curve replot when a single point
 * is added.
 *
 * setBoundsAuto enables tracking minimum and maximum *of the "Y" data*. X data, if specified
 * with the appropriate *append*, is assumed to be provided already ordered (ascending), so that
 * QuCircularBuf::x0 returns x_data[0] and QuCircularBuf::xN returns x_data[x_data.size() - 1].
 *
 * \par X auto mode (x scale is automatically generated)
 * If the *y only* version of append is used, then x data is automatically assumed to be a vector
 * [ 0, 1, ..., y.size() -1 ]
 *
 * QuCircularBuf::append(double *xx, double *yy, size_t count) immediately disables the *x auto*
 * mode and assumes all subsequent calls to append use the same signature.
 * On the other hand, QuCircularBuf::append(double *yy, size_t count) assumes *x auto* is true,
 * and does nothing otherwise (for example, if the (x,y,count) signature version is called by mistake).
 *
 * \note the two flavours of append *shall not be mixed*!
 */
class QuCircularBuf : public QuBufBase
{
public:
    enum Durations { Hour_Secs = 60 * 60, Hour_Ms = Hour_Secs * 1000,
                 SixHours_Secs = 12 * Hour_Secs, SixHours_Ms = Hour_Ms * 12,
                 TwelveHours_Secs = 12 * Hour_Secs, TwelveHours_Ms = Hour_Ms * 12,
                 Day_Secs = 24 * Hour_Secs, Day_Ms = Day_Secs * 1000,
                 Week_Secs = Day_Secs * 7, Week_Ms = Day_Ms * 7 };

    QuCircularBuf(bool xauto, bool yauto, size_t siz = Day_Secs);
    virtual ~QuCircularBuf();

    void init(size_t bufsiz);
    double x0() const;
    double xN() const;

    bool x_auto() const;
    size_t first() const;

    const QPointF p(size_t i) const;
    const QPointF operator [] (size_t i) const;

    double px(size_t i) const;
    double py(size_t i) const;
    size_t index(size_t i) const;


    size_t resizebuf(size_t s);
    size_t bufsize() const;

    bool append(double *xx, double *yy, size_t count);
    bool append(double *yy, size_t count);

    void insert(size_t idx, double *yy, size_t count);
    void insert(size_t idx, double *xx, double *yy, size_t count);

    void setBoundsAuto(bool x, bool y);
    bool xBoundsAuto();
    bool yBoundsAuto();

    std::vector<double> x, y;

    // QwtSeriesData interface
public:
    size_t size() const;
    QPointF sample(size_t i) const;
    QRectF boundingRect() const;

    bool m_xb_calc();

private:
    QuPlotDataBufP *d;
};

#endif // QUCIRCULARBUF_H
