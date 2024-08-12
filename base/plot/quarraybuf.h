#ifndef QUARRAYBUF_H
#define QUARRAYBUF_H
#include <qubufoptions.h>
#include <qubufbase.h>

/*!
 * \brief A linear buffer to store an array of *Y* data, exposing both the
 * *copy* and *move* semantics.
 *
 * Implements *QwtSeriesData< QPointF >* interface, and the *size*,
 * *sample(size_t)* and *boundingRect* methods.
 *
 */
class QuArrayBuf : public QuBufBase
{
public:
    QuArrayBuf(bool xauto, bool yauto);

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


    inline void m_bounds_calc(bool xa, bool ya, bool datasiz_changed);
};

#endif // QUARRAYBUF_H
