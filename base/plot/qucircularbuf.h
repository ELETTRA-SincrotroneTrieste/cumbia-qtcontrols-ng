#ifndef QUCIRCULARBUF_H
#define QUCIRCULARBUF_H

#include <qwt_series_data.h>

class QuPlotDataBufP;


class QuCircularBuf : public QwtSeriesData< QPointF >
{
public:
    QuCircularBuf(size_t siz = 0);
    virtual ~QuCircularBuf();

    void init(size_t bufsiz);
    double x0() const;
    double xN() const;

    bool x_auto() const;
    size_t first() const;

    QPointF p(size_t i) const;
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

private:
    QuPlotDataBufP *d;
};

#endif // QUCIRCULARBUF_H
