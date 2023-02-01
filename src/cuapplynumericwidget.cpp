#include "cuapplynumericwidget.h"
#include "cuapplynumericengine.h"

#include <QPaintEvent>
#include <QPainter>
#include <math.h>

class CuApplyNumericWidgetPrivate {
public:
    CuApplyNumericEngine e;
};

CuApplyNumericWidget::CuApplyNumericWidget(QWidget *parent)
    : QWidget{parent} {
    d = new CuApplyNumericWidgetPrivate;
}

CuApplyNumericWidget::~CuApplyNumericWidget() {
    delete d;
}


QSize CuApplyNumericWidget::sizeHint() const {
    return QSize(100, 40);
}

QSize CuApplyNumericWidget::minimumSizeHint() const {
    return QSize(100, 40);
}

double CuApplyNumericWidget::value() const { return d->e.d.data*pow(10.0, -d->e.d.decDig); }

double CuApplyNumericWidget::maximum() const { return d->e.d.d_maxAsDouble; }

double CuApplyNumericWidget::minimum() const { return d->e.d.d_minAsDouble; }

int CuApplyNumericWidget::intDigits() const { return d->e.d.intDig; }

int CuApplyNumericWidget::decDigits() const { return d->e.d.decDig; }

void CuApplyNumericWidget::setIntDigits(int i)
{
    if (i < 1)
        return;
    d->e.d.intDig = i;
    d->e.d.digits = d->e.d.intDig + d->e.d.decDig;
    update();
}


void CuApplyNumericWidget::setDecDigits(int d)
{

}


void CuApplyNumericWidget::mousePressEvent(QMouseEvent *event)
{
}

void CuApplyNumericWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void CuApplyNumericWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void CuApplyNumericWidget::mouseMoveEvent(QMouseEvent *event)
{
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
    d->e.paint(&p, r, this);
}
