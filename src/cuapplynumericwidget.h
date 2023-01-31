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


    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // CUAPPLYNUMERICWIDGET_H
