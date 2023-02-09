#include "cuapplynumericwidget.h"
#include "cuapplynumericengine.h"
#include "cuapplynumericclickrepeater.h"

#include <QPaintEvent>
#include <QPainter>
#include <math.h>

class CuApplyNumericWidgetPrivate {
public:
    CuApplyNumericWidgetPrivate(const QFont& f) {
        e = new CuApplyNumericEngine(f);
    }
    ~CuApplyNumericWidgetPrivate() {
        delete e;
    }
    CuApplyNumericEngine *e;
    CuApplyNumericClickRepeater click_r;
};

CuApplyNumericWidget::CuApplyNumericWidget(QWidget *parent)
    : QWidget{parent} {
    d = new CuApplyNumericWidgetPrivate(font());
    setMouseTracking(true);

    connect(&d->click_r, SIGNAL(step(QPointF)), d->e, SLOT(step(QPointF)));
    connect(&d->click_r, SIGNAL(step(QPointF)), this, SLOT(update()));
    connect(d->e, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(d->e, SIGNAL(minimumChanged(double)), this, SIGNAL(minimumChanged(double)));
    connect(d->e, SIGNAL(maximumChanged(double)), this, SIGNAL(maximumChanged(double)));
    connect(d->e, SIGNAL(intDigitsChanged(int)), this, SIGNAL(intDigitsChanged(int)));
    connect(d->e, SIGNAL(decDigitsChanged(int)), this, SIGNAL(decDigitsChanged(int)));
}

CuApplyNumericWidget::~CuApplyNumericWidget() {
    delete d;
}


QSize CuApplyNumericWidget::sizeHint() const {
    return d->e->minimumSize().toSize();
}

QSize CuApplyNumericWidget::minimumSizeHint() const {
    return d->e->minimumSize().toSize();
}

double CuApplyNumericWidget::value() const { return d->e->d.data*pow(10.0, -d->e->d.decDig); }

double CuApplyNumericWidget::maximum() const { return d->e->maximum(); }

double CuApplyNumericWidget::minimum() const { return d->e->minimum(); }

int CuApplyNumericWidget::intDigits() const { return d->e->d.intDig; }

int CuApplyNumericWidget::decDigits() const { return d->e->d.decDig; }

void CuApplyNumericWidget::setValue(double v) {
    d->e->setValue(v);
    update();
}

void CuApplyNumericWidget::setMaximum(double v) {
    d->e->setMaximum(v);
    update();
}

void CuApplyNumericWidget::setMinimum(double v) {
    d->e->setMinimum(v);
    update();
}

void CuApplyNumericWidget::setIntDigits(int i) {
    d->e->setIntDigits(i);
    update();
}


void CuApplyNumericWidget::setDecDigits(int dd) {
    d->e->setDecDigits(dd);
    update();

}

void CuApplyNumericWidget::mousePressEvent(QMouseEvent *event) {
    d->e->mousePressEvent(event->pos());
    d->click_r.pressed(event->pos());
    update();
}

void CuApplyNumericWidget::mouseReleaseEvent(QMouseEvent *event) {
    d->e->mouseReleaseEvent(d->click_r.active() ? QPointF() : event->pos());
    d->click_r.released(event->pos());
    update();
}

void CuApplyNumericWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void CuApplyNumericWidget::mouseMoveEvent(QMouseEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void CuApplyNumericWidget::wheelEvent(QWheelEvent *event)
{
}

void CuApplyNumericWidget::enterEvent(QEvent *event)
{
}

void CuApplyNumericWidget::leaveEvent(QEvent *event)
{
}

void CuApplyNumericWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect r = rect();
    r.setRight(r.right() - 1);
    r.setBottom(r.bottom() - 1);
    d->e->paint(&p, r, this);
}

void CuApplyNumericWidget::changeEvent(QEvent *e) {
    if(e->type() == QEvent::FontChange) {
        d->e->setFont(font());
    }
    QWidget::changeEvent(e);
}
