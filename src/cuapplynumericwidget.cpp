#include "cuapplynumericwidget.h"

class CuApplyNumericWidgetPrivate {
public:
};

CuApplyNumericWidget::CuApplyNumericWidget(QWidget *parent)
    : QWidget{parent} {
    d = new CuApplyNumericWidgetPrivate;
}

CuApplyNumericWidget::~CuApplyNumericWidget() {
    delete d;
}
