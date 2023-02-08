#ifndef CUAPPLYNUMERICWIDGET_H
#define CUAPPLYNUMERICWIDGET_H

#include <QWidget>

class CuApplyNumericWidgetPrivate;

class CuApplyNumericWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int integerDigits READ intDigits WRITE setIntDigits)
    Q_PROPERTY(int decimalDigits READ decDigits WRITE setDecDigits)
    Q_PROPERTY(double value READ value WRITE setValue)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(double fontScale READ fontScale WRITE setFontScale)
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

    double maximum() const;
    double value() const;
    double minimum() const;
    int intDigits() const;
    int decDigits() const;
    double fontScale() const;

public slots:
    void setValue(double v);
    void setFontScale(double scale);

    void setMaximum(double v);
    void setMinimum(double v);
    /**
     * returns the minimum value of the widget
     * @return min val
     */

    /**
     * sets number of integer digits to be displayed
     * @param i	number of digits
     */
    void setIntDigits(int i);

    /**
     * sets number of deciam digits to be displayed
     * @param d	number of digits
     */
    void setDecDigits(int d);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

    void changeEvent(QEvent *e);
};

#endif // CUAPPLYNUMERICWIDGET_H
