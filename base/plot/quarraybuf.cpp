#include "quarraybuf.h"
#include "cumacros.h"

QuArrayBuf::QuArrayBuf(bool xauto, bool yauto)
    : QuBufBase(xauto, yauto), datasiz{0} {
}


size_t QuArrayBuf::size() const {
    return datasiz;
}

QPointF QuArrayBuf::sample(size_t i) const {
    return (x.size() == y.size() && i < datasiz) ?
               QPointF{x[i], y[i]} : QPointF{static_cast<double>(i), y[i]};
}

QRectF QuArrayBuf::boundingRect() const {
    return QRectF(o.xlb, o.ylb, o.xub - o.xlb, o.yub - o.ylb);
}

void QuArrayBuf::move(const std::vector<double> &ya) {
    const size_t &s = ya.size();
    y = std::move(ya);
    m_bounds_calc(o.xb_auto, o.yb_auto, datasiz != s);
    datasiz = s;
}

void QuArrayBuf::set(const std::vector<double> &ya) {
    const size_t &s = ya.size();
    y = ya;
    m_bounds_calc(o.xb_auto, o.yb_auto, datasiz != s); // inline
    datasiz = ya.size();
}

inline void QuArrayBuf::m_bounds_calc(bool xa, bool ya, bool datasiz_changed) {
    if(ya) { // must find min and max in yy
        auto [a, b] = std::minmax_element(y.begin(), y.end());
        o.ylb = *a.base();
        o.yub = *b.base();
    }
    if(xa && x.size() == y.size()) {
        o.xlb = x[0];
        o.xub = x[datasiz - 1];
    } else if(xa && datasiz_changed) {
        o.xlb = 0;
        o.xub = y.size() - 1;
    }
}
