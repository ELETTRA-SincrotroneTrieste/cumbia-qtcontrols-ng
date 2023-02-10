#ifndef QUAPPLYNUMERICWIDGET_H
#define QUAPPLYNUMERICWIDGET_H

#include <QWidget>

class QuApplyNumericWidgetPrivate;

class QuApplyNumericWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int intDigits READ intDigits WRITE setIntDigits)
    Q_PROPERTY(int decDigits READ decDigits WRITE setDecDigits)
    Q_PROPERTY(double value READ value WRITE setValue)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(bool showApply READ showApply WRITE setShowApply)
public:
    explicit QuApplyNumericWidget(QWidget *parent = nullptr);
    virtual  ~QuApplyNumericWidget();

signals:
    void valueChanged(double v);
    void minimumChanged(double m);
    void maximumChanged(double M);
    void intDigitsChanged(int id);
    void decDigitsChanged(int dd);

private:
    QuApplyNumericWidgetPrivate *d;


    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    double maximum() const;
    double value() const;
    double minimum() const;
    int intDigits() const;
    int decDigits() const;

    bool showApply() const;

public slots:
    void setValue(double v);

    void setShowApply(bool s);

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
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

    void changeEvent(QEvent *e);
};

#endif // QUAPPLYNUMERICWIDGET_H
