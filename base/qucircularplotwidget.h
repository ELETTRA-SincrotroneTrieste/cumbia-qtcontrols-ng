#ifndef QUCIRCULARPLOTWIDGET_H
#define QUCIRCULARPLOTWIDGET_H

#include <QWidget>

class QuCircularPlotW_P;

class QuCircularPlotW : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
public:
    explicit QuCircularPlotW(QWidget *parent = nullptr);
    virtual  ~QuCircularPlotW();

signals:
    void minimumChanged(double m);
    void maximumChanged(double M);

private:
    QuCircularPlotW_P *d;


    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    double maximum() const;
    double minimum() const;

public slots:

    void setMaximum(double v);
    void setMinimum(double v);

    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

    void changeEvent(QEvent *e);
};

#endif // QUCIRCULARPLOTWIDGET_H
