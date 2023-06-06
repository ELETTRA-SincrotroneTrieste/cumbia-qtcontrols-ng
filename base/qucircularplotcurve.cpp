#include "qucircularplotcurve.h"
#include "cumacros.h"

class QuCircularPlotCurveP {
public:
    QuCircularPlotCurveP(const QString& s, QuCircularPlotCurveListener *l)
        : src(s), min(0.0), max(0.0), selected(-1), editable(false), cl(l) {

    }
    QString src;
    QVector<double> x, y;
    double min, max;
    QPen pen;
    int selected;
    bool editable;
    QuCircularPlotCurveListener *cl;
};

QuCircularPlotCurve::QuCircularPlotCurve(const QString& src, QuCircularPlotCurveListener *l, const QPen& p)
    : d(new QuCircularPlotCurveP(src, l)) {
    d->pen = p;
}

QuCircularPlotCurve::~QuCircularPlotCurve() {
    delete d;
}

const QVector<double> &QuCircularPlotCurve::x_data() const {
    return d->x;
}

const QVector<double> &QuCircularPlotCurve::y_data() const {
    return d->y;
}

QString QuCircularPlotCurve::source() const {
    return d->src;
}

// set data on curves and always update min max even if Y autoscale is disabled
// because we always need to draw the curves accurately in our circular space
void QuCircularPlotCurve::setData(const QVector<double> &x, const QVector<double> &y) {
    if(x.size() == y.size() && x.size() > 0) {
        d->x = std::move(x);
        d->y = std::move(y);
        const auto [mi, ma] = std::minmax_element(d->y.begin(), d->y.end());
        bool bc = *mi != d->min || *ma != d->max;
        d->min = *mi; d->max = *ma;
        if(bc) d->cl->onBoundsChanged();
    }
}

int QuCircularPlotCurve::size() const {
    return d->x.size();
}

void QuCircularPlotCurve::setPen(const QPen &p) {
    d->pen = p;
}

QPen QuCircularPlotCurve::pen() const {
    return d->pen;
}

double QuCircularPlotCurve::ymax() const {
    return d->max;
}

double QuCircularPlotCurve::ymin() const {
    return d->min;
}

void QuCircularPlotCurve::minmax_update() {
    const auto [mi, ma] = std::minmax_element(d->y.begin(), d->y.end());
    d->min = *mi; d->max = *ma;
}

void QuCircularPlotCurve::select(int idx) {
    d->selected = idx;
}

void QuCircularPlotCurve::deselect() {
    d->selected = -1;
}

int QuCircularPlotCurve::selected() const {
    return d->selected;
}

bool QuCircularPlotCurve::isEditable() const {
    return d->editable;
}

void QuCircularPlotCurve::setEditable(bool e) {
    d->editable = e;
}
