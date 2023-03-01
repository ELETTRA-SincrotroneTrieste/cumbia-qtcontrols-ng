#include "qunumericwidget.h"
#include "qunumericengine.h"
#include "qunumericclickrepeater.h"

#include <QPaintEvent>
#include <QPainter>
#include <math.h>

class QuNumericW_P {
public:
    QuNumericW_P(const QFont& f) : e(new QuNumericEngine(f)) {
    }
    ~QuNumericW_P() {
        delete e;
    }
    QuNumericEngine *e;
    QuNumericClickRepeat click_r;
};

QuNumericW::QuNumericW(QWidget *parent)
    : QWidget{parent} {
    d = new QuNumericW_P(font());
    setMouseTracking(true);

    connect(&d->click_r, SIGNAL(step(QPointF)), d->e, SLOT(click(QPointF)));
    connect(&d->click_r, SIGNAL(step(QPointF)), this, SLOT(update()));
    connect(d->e, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(d->e, SIGNAL(minimumChanged(double)), this, SIGNAL(minimumChanged(double)));
    connect(d->e, SIGNAL(maximumChanged(double)), this, SIGNAL(maximumChanged(double)));
    connect(d->e, SIGNAL(intDigitsChanged(int)), this, SIGNAL(intDigitsChanged(int)));
    connect(d->e, SIGNAL(decDigitsChanged(int)), this, SIGNAL(decDigitsChanged(int)));
    connect(d->e, SIGNAL(applyClicked(double)), this, SIGNAL(clicked(double)));
}

QuNumericW::~QuNumericW() {
    delete d;
}


QSize QuNumericW::sizeHint() const {
    return d->e->minimumSize().toSize();
}

QSize QuNumericW::minimumSizeHint() const {
    return d->e->minimumSize().toSize();
}

double QuNumericW::value() const { return d->e->d.data*pow(10.0, -d->e->d.decDig); }

double QuNumericW::maximum() const { return d->e->maximum(); }

double QuNumericW::minimum() const { return d->e->minimum(); }

int QuNumericW::intDigits() const { return d->e->d.intDig; }

int QuNumericW::decDigits() const { return d->e->d.decDig; }

bool QuNumericW::showApply() const {
    return d->e->d.show_apply;
}

void QuNumericW::setValue(double v) {
    d->e->setValue(v);
    update();
}

void QuNumericW::setShowApply(bool s) {
    d->e->setShowApply(s);
    update();
    updateGeometry(); // changes minimum size
}

void QuNumericW::setMaximum(double v) {
    d->e->setMaximum(v);
    update();
}

void QuNumericW::setMinimum(double v) {
    d->e->setMinimum(v);
    update();
}

void QuNumericW::setIntDigits(int i) {
    d->e->setIntDigits(i);
    update();
}


void QuNumericW::setDecDigits(int dd) {
    d->e->setDecDigits(dd);
    update();

}

void QuNumericW::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mousePressEvent(event->pos());
        d->click_r.pressed(event->pos());
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuNumericW::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        d->e->mouseReleaseEvent(d->click_r.active() ? QPointF() : event->pos());
        d->click_r.released(event->pos());
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QuNumericW::mouseDoubleClickEvent(QMouseEvent *event) {

}

void QuNumericW::mouseMoveEvent(QMouseEvent *event) {
    d->e->mouseMoveEvent(event->pos());
    update();
}

void QuNumericW::wheelEvent(QWheelEvent *event) {
    d->e->wheelEvent(event->position(), event->angleDelta().y());
    update();
}

void QuNumericW::leaveEvent(QEvent *event)
{
    d->e->mouseReleaseEvent(QPointF());
    d->click_r.released(QPointF());
    update();
    QWidget::leaveEvent(event);
}

void QuNumericW::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect r = rect();
    r.setRight(r.right() - 1);
    r.setBottom(r.bottom() - 1);
    d->e->paint(&p, r, this);
}

void QuNumericW::changeEvent(QEvent *e) {
    if(e->type() == QEvent::FontChange) {
        d->e->setFont(font());
    }
    QWidget::changeEvent(e);
}
