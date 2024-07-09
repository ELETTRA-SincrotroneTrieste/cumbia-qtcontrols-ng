#ifndef QUARRAYBUF_H
#define QUARRAYBUF_H
#include <qwt_series_data.h>
#include <qubufoptions.h>

class QuArrayBuf : public QwtSeriesData< QPointF >
{
public:
    QuArrayBuf();

    // QwtSeriesData interface
public:
    size_t size() const;
    QPointF sample(size_t i) const;
    QRectF boundingRect() const;

    void move(const std::vector<double>& y);
    void set(const std::vector<double>& y);

private:
    std::vector<double> x, y;
    size_t datasiz;
    QuBuf_O o;


    inline void m_bounds_calc(bool xa, bool ya);
};

#endif // QUARRAYBUF_H
