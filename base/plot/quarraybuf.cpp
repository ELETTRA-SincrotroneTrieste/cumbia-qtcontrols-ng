#include "quarraybuf.h"
#include "cumacros.h"

QuArrayBuf::QuArrayBuf() {}


size_t QuArrayBuf::size() const {
    return datasiz;
}

QPointF QuArrayBuf::sample(size_t i) const {
    return (x.size() == y.size() && i < datasiz) ?
               QPointF{x[i], y[i]} : QPointF{static_cast<double>(i), y[i]};
}

QRectF QuArrayBuf::boundingRect() const {
    pretty_pri("rect (%.1f,%.1f %.1fx%.1f\n", o.xlb, o.ylb, o.xub - o.xlb, o.yub - o.ylb);
    return QRectF(o.xlb, o.ylb, o.xub - o.xlb, o.yub - o.ylb);
}

void QuArrayBuf::move(const std::vector<double> &ya) {
    datasiz = ya.size();
    y = std::move(ya);
    m_bounds_calc(o.xb_auto, o.yb_auto);
}

void QuArrayBuf::set(const std::vector<double> &ya) {
    datasiz = ya.size();
    y = ya;
    m_bounds_calc(o.xb_auto, o.yb_auto);
    if(o.yb_auto) { // must find min and max in yy
        auto [a, b] = std::minmax_element(y.begin(), y.end());
        o.ylb = *a.base();
        o.yub = *b.base();
    }
}

void QuArrayBuf::m_bounds_calc(bool xa, bool ya) {
    if(ya) { // must find min and max in yy
        auto [a, b] = std::minmax_element(y.begin(), y.end());
        o.ylb = *a.base();
        o.yub = *b.base();
    }
    if(xa && x.size() == datasiz) {
        o.xlb = x[0];
        o.xub = x[datasiz - 1];
    }
}
