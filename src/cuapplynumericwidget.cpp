#include "cuapplynumericwidget.h"
#include "cuapplynumericengine.h"

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


QSize CuApplyNumericWidget::sizeHint() const
{
}

QSize CuApplyNumericWidget::minimumSizeHint() const
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

void CuApplyNumericWidget::paintEvent(QPaintEvent *event)
{
}
