#ifndef QUCIRCULARBUF_H
#define QUCIRCULARBUF_H

#include <qubufbase.h>
#include <qwt_series_data.h>

class QuPlotDataBufP;


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

    QPointF p(size_t i) const;
    double px(size_t i) const;
    double py(size_t i) const;

    size_t resizebuf(size_t s);
    size_t bufsize() const;

    void append(double *xx, double *yy, size_t count);
    void append(double *yy, size_t count);

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

    void m_xb_calc();

private:
    QuPlotDataBufP *d;
};

#endif // QUCIRCULARBUF_H
