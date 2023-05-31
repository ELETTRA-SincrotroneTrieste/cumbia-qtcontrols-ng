#include "qucircularplotwidget.h"
#include "qucircularplotengine.h"

#include <QPaintEvent>
#include <QPainter>
#include <math.h>
#include "quzoomer.h"
#include "quzoomevents.h"
#include "qucircularplotcurveselectionevents.h"

class QuCircularPlotW_P {
public:
    QuCircularPlotW_P(QWidget *o, const QFont& f) {
        QuCircularPlotCurveSelectionEvents *cse = new QuCircularPlotCurveSelectionEvents(o);
        QuZoomEvents *ze = new QuZoomEvents(o);
        e = new QuCircularPlotEngine(f, new QuZoomer(o), cse, ze);
        // circular plot engine takes the ownership of zoomer and zoom events
        o->installEventFilter(ze); // 2nd
        o->installEventFilter(cse); // activated first 1st
    }
    ~QuCircularPlotW_P() {
        delete e;
    }
    QuCircularPlotEngine *e;
};

QuCircularPlotW::QuCircularPlotW(QWidget *parent)
    : QWidget{parent} {
    d = new QuCircularPlotW_P(this, font());
    setMouseTracking(true);
    connect(d->e->zoomer(), SIGNAL(zoomChanged()), this, SLOT(update()));
}

QuCircularPlotW::~QuCircularPlotW() {
    delete d;
}


QSize QuCircularPlotW::sizeHint() const {
    return d->e->minimumSize().toSize();
}

QSize QuCircularPlotW::minimumSizeHint() const {
    return d->e->minimumSize().toSize();
}

double QuCircularPlotW::maximum() const { return d->e->maximum(); }

double QuCircularPlotW::minimum() const { return d->e->minimum(); }

void QuCircularPlotW::setMaximum(double v) {
    d->e->setYLowerBound(v);
    update();
}

void QuCircularPlotW::setMinimum(double v) {
    d->e->setYUpperBound(v);
    update();
}

void QuCircularPlotW::setData(const QString &src, const QVector<double> &xdata, const QVector<double> &ydata)
{

}

void QuCircularPlotW::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuCircularPlotW::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        update();

    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuCircularPlotW::mouseDoubleClickEvent(QMouseEvent *event) {

}

void QuCircularPlotW::mouseMoveEvent(QMouseEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void QuCircularPlotW::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
}

void QuCircularPlotW::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect r = rect();
    r.setRight(r.right() - 1);
    r.setBottom(r.bottom() - 1);
    d->e->paint(&p, r, this);
}

void QuCircularPlotW::changeEvent(QEvent *e) {
    if(e->type() == QEvent::FontChange) {
        d->e->setFont(font());
    }
    QWidget::changeEvent(e);
}
