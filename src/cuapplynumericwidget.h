#ifndef CUAPPLYNUMERICWIDGET_H
#define CUAPPLYNUMERICWIDGET_H

#include <QWidget>

class CuApplyNumericWidgetPrivate;

class CuApplyNumericWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CuApplyNumericWidget(QWidget *parent = nullptr);
    virtual  ~CuApplyNumericWidget();

signals:

private:
    CuApplyNumericWidgetPrivate *d;

};

#endif // CUAPPLYNUMERICWIDGET_H
