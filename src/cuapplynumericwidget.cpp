#include "cuapplynumericwidget.h"
#include "cuapplynumericengine.h"

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
};

CuApplyNumericWidget::CuApplyNumericWidget(QWidget *parent)
    : QWidget{parent} {
    d = new CuApplyNumericWidgetPrivate(font());
    setMouseTracking(true);
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

double CuApplyNumericWidget::maximum() const { return d->e->d.d_maxAsDouble; }

double CuApplyNumericWidget::minimum() const { return d->e->d.d_minAsDouble; }

int CuApplyNumericWidget::intDigits() const { return d->e->d.intDig; }

int CuApplyNumericWidget::decDigits() const { return d->e->d.decDig; }

double CuApplyNumericWidget::fontScale() const {
    return d->e->d.font_scale;
}

void CuApplyNumericWidget::setValue(double v)
{

}

void CuApplyNumericWidget::setFontScale(double scale) {
    d->e->d.font_scale = scale;
    update();
}

void CuApplyNumericWidget::setMaximum(double v)
{

}

void CuApplyNumericWidget::setMinimum(double v)
{

}

void CuApplyNumericWidget::setIntDigits(int i)
{
    if (i < 1)
        return;
    d->e->d.intDig = i;
    d->e->d.digits = d->e->d.intDig + d->e->d.decDig;
    update();
}


void CuApplyNumericWidget::setDecDigits(int d)
{

}


void CuApplyNumericWidget::mousePressEvent(QMouseEvent *event) {
    d->e->mousePressEvent(event->pos());
    update();
}

void CuApplyNumericWidget::mouseReleaseEvent(QMouseEvent *event) {
    d->e->mouseReleaseEvent(event->pos());
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
