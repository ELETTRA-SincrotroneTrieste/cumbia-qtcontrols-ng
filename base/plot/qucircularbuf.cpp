#include "qucircularbuf.h"
#include <QDateTime>
#include <cumacros.h>

class QuPlotDataBufP {
public:
    QuPlotDataBufP() : first(0), xax_auto(true) {}
    size_t bufsiz, first, datasiz;
    bool xax_auto; // x axis auto populate
};

QuCircularBuf::QuCircularBuf(bool xauto, bool yauto, size_t siz) : QuBufBase(xauto, yauto) {
    d = new QuPlotDataBufP();
    init(siz);
}

QuCircularBuf::~QuCircularBuf() {
    delete d;
}

/*!
 * \brief intialize x with values from 0 to siz - 1, y with zeroes
 * \param siz the *buffer* size
 */
void QuCircularBuf::init(size_t bufsiz) {
    y.resize(bufsiz, 0);
    d->bufsiz = bufsiz;
    d->datasiz = d->first = 0;
    o.ylb = 1.0; o.yub = -1.0; // min > max forces initialization
    o.xlb = 1.0; o.xub = -1.0;
}

double QuCircularBuf::x0() const {
    return d->xax_auto ? d->first : p(0).x();
}

double QuCircularBuf::xN() const {
    return d->datasiz > 0 ? p(d->datasiz - 1).x() : -1;
}

bool QuCircularBuf::x_auto() const {
    return d->xax_auto;
}

size_t QuCircularBuf::first() const {
    return d->first;
}

QPointF QuCircularBuf::p(size_t i) const {
    QPointF r(-1.0, -1.0);
    if(i >= d->datasiz)
        return r;
    size_t idx = (d->first + i) % d->bufsiz;
    return QPointF(d->xax_auto ? i : x[idx], y[idx]);
}

double QuCircularBuf::py(size_t i) const {
    if(i >= d->datasiz)
        return -1;
    size_t idx = (d->first + i) % d->bufsiz;
    return  y[idx];
}

double QuCircularBuf::px(size_t i) const {
    if(i >= d->datasiz)
        return -1;
    size_t idx = (d->first + i) % d->bufsiz;
    printf("\e[1;33m i %ld idx %ld x size %ld x[idx] %f xax_auto? %s\e[0m\n", i, idx, d->datasiz, x[idx],
d->xax_auto ? "YES" : "NO");
    return  d->xax_auto ? i : x[idx];
}

size_t QuCircularBuf::size() const {
    return d->datasiz;
}

size_t QuCircularBuf::bufsize() const {
    return d->bufsiz;
}

QPointF QuCircularBuf::sample(size_t i) const {
    return d->xax_auto ?  QPointF(i, py(i)) : p(i);
}

QRectF QuCircularBuf::boundingRect() const {
    double x0 = o.xlb > o.xub ? 0 : o.xlb,
        y0 = o.ylb > o.yub ? 0 : o.ylb,
        w = o.xlb > o.xub ? 1000 : o.xub - o.xlb,
        h = o.ylb > o.yub ? 1000 : o.yub - o.ylb;

    pretty_pri("rect (%.1f,%.1f %.1fx%.1f\n", x0, y0 , w, h);
    return QRectF(x0, y0, w, h);
}

void QuCircularBuf::m_xb_calc() {
    // width: extend past last x value to allow incremental painting
    const size_t min_dsiz = 10, samples = 10;
    const double& X = px(d->datasiz - 1);
    if(d->datasiz > min_dsiz && o.xub < X) {
        // average the last 10 distances between subsequent x axis updates
        double avg = 0;
        int cnt = 0;
        for(size_t i = d->datasiz - 1; i > d->datasiz - samples - 1; i--) {
            avg += px(i) - px(i-1);
            cnt++;
        }
        avg /= samples;
        o.xlb = px(0);
        double oldub = o.xub;
        o.xub = px(d->datasiz - 1) + 10 * avg; // extend for about 10 next readings
        pretty_pri("\e[1;31mextending x axis: old upper %s new upper %s avg %f last %ld samples averaged %d \e[0m",
                   qstoc(QDateTime::fromMSecsSinceEpoch(oldub).toString("hh.MM.ss")),
                   qstoc(QDateTime::fromMSecsSinceEpoch(o.xub).toString("hh.MM.ss")),
                   avg, samples, cnt);
    } else if(d->datasiz < min_dsiz) {
        // initialize lower bound, then leave it unchanged until the buffer is full
        if(d->datasiz == 1) {
            pretty_pri("initializing lower bound to %s", qstoc(QDateTime::fromMSecsSinceEpoch(px(0)).toString()));
            o.xlb = px(0);
        }
        o.xub = px(d->datasiz - 1);
    }
}

/*!
 * \brief resizes the *buffer* to new size s.
 *
 * If new size < old size, the *tail* of data is preserved
 *
 * \return new size - old size
 */
size_t QuCircularBuf::resizebuf(size_t s) {
    size_t oldsiz(d->bufsiz);
    std::vector<double> X, Y;
    if(s >= d->bufsiz) {
        // re-arrange elements so that d->first is 0
        X.resize(d->xax_auto ? 0 : d->datasiz, 0);
        Y.resize(d->datasiz, 0);
        for(size_t i = 0, j = 0; i < d->datasiz; i++, j++) {
            if(!d->xax_auto)
                X[j] = x[d->first + i % d->bufsiz];
            Y[j] = y[d->first + i % d->bufsiz];
        }
    }
    else { // smaller size: preserve tail
        X.resize(d->xax_auto ? 0 : s, 0);
        Y.resize(std::min(s, d->datasiz), 0);
        // save tail into Y (X)
        for(int i = d->datasiz - 1, j = Y.size() - 1; j >= 0 && i >= 0; i--, j--) {
            const QPointF& xy = p(i);
            if(!d->xax_auto)
                X[j] = xy.x();
            Y[j] = xy.y();
        }
        d->datasiz = Y.size();
    }
    if(!d->xax_auto)
        x = std::move(X);
    y = std::move(Y);
    if(s >= d->bufsiz) {
        if(!d->xax_auto) x.resize(s);
        y.resize(s);
    }
    d->bufsiz = s;
    d->first = 0;
    return d->bufsiz - oldsiz;
}

void QuCircularBuf::append(double *xx, double *yy, size_t count) {
    d->xax_auto = false;
    if(d->datasiz == 0)
        x.resize(y.size());
    size_t next = (d->first + d->datasiz) % d->bufsiz;
    for(size_t i = 0; i < count; i++ ) {
        x[next] = xx[i];
        y[next] = yy[i];
        if(d->datasiz < d->bufsiz)
            d->datasiz++;
        else
            d->first = (d->first + 1) % d->bufsiz;
        next = (next + 1) % d->bufsiz;

        if(o.yb_auto) {
            if(o.ylb > o.yub)
                o.ylb = o.yub = yy[i];
            else if(yy[i] < o.ylb )
                o.ylb = yy[i];
            else if(yy[i] > o.yub)
                o.yub = yy[i];
            pretty_pri("\e[1;32myb %.1f  yub %.1f\e[0m", o.ylb, o.yub);
        }
    }
    if(o.xb_auto && d->datasiz > 0)
        m_xb_calc();
}

void QuCircularBuf::append(double *yy, size_t count) {
    if(d->xax_auto) {
        size_t next = (d->first + d->datasiz) % d->bufsiz;
        for(size_t i = 0; i < count; i++ ) {
            y[next] = yy[i];
            if(d->datasiz < d->bufsiz)
                d->datasiz++;
            else
                d->first = (d->first + 1) % d->bufsiz;
            next = (next + 1) % d->bufsiz;
            // update y bounds if necessary
            if(o.yb_auto) {
                if(o.ylb > o.yub)
                    o.ylb = o.yub = yy[i];
                else if(yy[i] < o.ylb )
                    o.ylb = yy[i];
                else if(yy[i] > o.yub)
                    o.yub = yy[i];
            }
        }
    }
}

/*!
 * \brief insert values at pos idx
 *
 * y is extended by inserting new elements before the element at the
 * specified position, increasing the data size by the number of
 * elements inserted.
 *
 * \param idx position where to insert. If greater than *size*, then
 *        data shall be appended at the end. If less than 0, it shall
 *        be inserted before current data.
 * \param yy pointer to data
 * \param count number of elements in yy
 */
void QuCircularBuf::insert(size_t idx, double *yy, size_t count) {
    if(d->xax_auto) {
        if(idx > d->datasiz) idx = d->datasiz;
        if(idx < 0) idx = 0;
        if(d->bufsiz >= d->datasiz + count) {
            // there is enough space for data: use std vector insert
            y.insert(y.begin() + idx, yy, yy + count);
            d->datasiz += count;
        } else {
            // preserve the tail: map idx
            size_t tlen = d->datasiz - idx;
            std::vector<double> Y(tlen);
            for(size_t i = 0; i < tlen; i++) {
                size_t j = (d->first + i + idx) % d->bufsiz;
                Y[i] = y[j];
            } // Y contains the tail
            size_t next = (d->first + idx) % d->bufsiz;
            // append yy starting from idx (datasiz)
            for(size_t i = 0; i < count; i++ ) {
                y[next] = yy[i];
                idx < d->bufsiz ? idx++ :
                    d->first = (d->first + 1) % d->bufsiz;
                next = (next + 1) % d->bufsiz;
                // update y bounds if necessary
                if(o.yb_auto) {
                    if(o.ylb > o.yub)
                        o.ylb = o.yub = yy[i];
                    else if(yy[i] < o.ylb )
                        o.ylb = yy[i];
                    else if(yy[i] > o.yub)
                        o.yub = yy[i];
                }
            }
            // append saved tail
            for(size_t i = 0; i < tlen; i++) {
                y[next] = Y[i];
                idx < d->bufsiz ? idx++ :
                    d->first = (d->first + 1) % d->bufsiz;
                next = (next + 1) % d->bufsiz;
            }
            d->datasiz = idx;
        }
    }
}

void QuCircularBuf::insert(size_t idx, double *xx, double *yy, size_t count)
{
    if(!d->xax_auto) {
        if(idx > d->datasiz) idx = d->datasiz;
        if(d->bufsiz >= d->datasiz + count) {
            // there is enough space for data: use std vector insert
            y.insert(y.begin() + idx, yy, yy + count);
            x.insert(x.begin() + idx, xx, xx + count);
            d->datasiz += count;
        } else {
            // preserve the tail: map idx
            size_t tlen = d->datasiz - idx;
            std::vector<double> Y(tlen), X(tlen);
            for(size_t i = 0; i < tlen; i++) {
                size_t j = (d->first + i + idx) % d->bufsiz;
                Y[i] = y[j];
                X[i] = x[j];
            } // Y contains the tail
            size_t next = (d->first + idx) % d->bufsiz;
            // append yy starting from idx (datasiz)
            for(size_t i = 0; i < count; i++ ) {
                y[next] = yy[i];
                x[next] = xx[i];
                idx < d->bufsiz ? idx++ :
                    d->first = (d->first + 1) % d->bufsiz;
                next = (next + 1) % d->bufsiz;
                // update y bounds if necessary
                if(o.yb_auto) {
                    if(o.ylb > o.yub)
                        o.ylb = o.yub = yy[i];
                    else if(yy[i] < o.ylb )
                        o.ylb = yy[i];
                    else if(yy[i] > o.yub)
                        o.yub = yy[i];
                }
            }
            // append saved tail
            for(size_t i = 0; i < tlen; i++) {
                y[next] = Y[i];
                x[next] = xx[i];
                idx < d->bufsiz ? idx++ :
                    d->first = (d->first + 1) % d->bufsiz;
                next = (next + 1) % d->bufsiz;
            }
            d->datasiz = idx;
        }
    }
}

void QuCircularBuf::setBoundsAuto(bool x, bool y) {
    o.xb_auto = x;
    o.yb_auto = y;
    if(y) { // reset min > max to force first initialization
        o.ylb = 1.0; o.yub = -1.0;
    }
}

bool QuCircularBuf::xBoundsAuto() {
    return o.xb_auto;
}

bool QuCircularBuf::yBoundsAuto() {
    return o.yb_auto;
}


