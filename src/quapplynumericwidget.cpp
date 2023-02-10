#include "quapplynumericwidget.h"
#include "quapplynumericengine.h"
#include "quapplynumericclickrepeater.h"

#include <QPaintEvent>
#include <QPainter>
#include <math.h>

class QuApplyNumericWidgetPrivate {
public:
    QuApplyNumericWidgetPrivate(const QFont& f) : e(new QuApplyNumericEngine(f)) {
    }
    ~QuApplyNumericWidgetPrivate() {
        delete e;
    }
    QuApplyNumericEngine *e;
    CuApplyNumericClickRepeater click_r;
};

QuApplyNumericWidget::QuApplyNumericWidget(QWidget *parent)
    : QWidget{parent} {
    d = new QuApplyNumericWidgetPrivate(font());
    setMouseTracking(true);

    connect(&d->click_r, SIGNAL(step(QPointF)), d->e, SLOT(click(QPointF)));
    connect(&d->click_r, SIGNAL(step(QPointF)), this, SLOT(update()));
    connect(d->e, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(d->e, SIGNAL(minimumChanged(double)), this, SIGNAL(minimumChanged(double)));
    connect(d->e, SIGNAL(maximumChanged(double)), this, SIGNAL(maximumChanged(double)));
    connect(d->e, SIGNAL(intDigitsChanged(int)), this, SIGNAL(intDigitsChanged(int)));
    connect(d->e, SIGNAL(decDigitsChanged(int)), this, SIGNAL(decDigitsChanged(int)));
}

QuApplyNumericWidget::~QuApplyNumericWidget() {
    delete d;
}


QSize QuApplyNumericWidget::sizeHint() const {
    return d->e->minimumSize().toSize();
}

QSize QuApplyNumericWidget::minimumSizeHint() const {
    return d->e->minimumSize().toSize();
}

double QuApplyNumericWidget::value() const { return d->e->d.data*pow(10.0, -d->e->d.decDig); }

double QuApplyNumericWidget::maximum() const { return d->e->maximum(); }

double QuApplyNumericWidget::minimum() const { return d->e->minimum(); }

int QuApplyNumericWidget::intDigits() const { return d->e->d.intDig; }

int QuApplyNumericWidget::decDigits() const { return d->e->d.decDig; }

bool QuApplyNumericWidget::showApply() const {
    return d->e->d.show_apply;
}

void QuApplyNumericWidget::setValue(double v) {
    d->e->setValue(v);
    update();
}

void QuApplyNumericWidget::setShowApply(bool s) {
    d->e->setShowApply(s);
    update();
    updateGeometry(); // changes minimum size
}

void QuApplyNumericWidget::setMaximum(double v) {
    d->e->setMaximum(v);
    update();
}

void QuApplyNumericWidget::setMinimum(double v) {
    d->e->setMinimum(v);
    update();
}

void QuApplyNumericWidget::setIntDigits(int i) {
    d->e->setIntDigits(i);
    update();
}


void QuApplyNumericWidget::setDecDigits(int dd) {
    d->e->setDecDigits(dd);
    update();

}

void QuApplyNumericWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        d->click_r.pressed(event->pos());
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuApplyNumericWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mouseReleaseEvent(d->click_r.active() ? QPointF() : event->pos());
        d->click_r.released(event->pos());
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuApplyNumericWidget::mouseDoubleClickEvent(QMouseEvent *event) {

}

void QuApplyNumericWidget::mouseMoveEvent(QMouseEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void QuApplyNumericWidget::wheelEvent(QWheelEvent *event) {
    d->e->wheelEvent(event->position(), event->angleDelta().y());
    update();
}

void QuApplyNumericWidget::leaveEvent(QEvent *event)
{
    d->e->mouseReleaseEvent(QPointF());
    d->click_r.released(QPointF());
    update();
    QWidget::leaveEvent(event);
}

void QuApplyNumericWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect r = rect();
    r.setRight(r.right() - 1);
    r.setBottom(r.bottom() - 1);
    d->e->paint(&p, r, this);
}

void QuApplyNumericWidget::changeEvent(QEvent *e) {
    if(e->type() == QEvent::FontChange) {
        d->e->setFont(font());
    }
    QWidget::changeEvent(e);
}
