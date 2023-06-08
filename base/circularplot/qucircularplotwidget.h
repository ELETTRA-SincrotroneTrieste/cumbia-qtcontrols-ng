#ifndef QUCIRCULARPLOTWIDGET_H
#define QUCIRCULARPLOTWIDGET_H

#include <QWidget>

class QuCircularPlotW_P;

class QuCircularPlotW : public QWidget
{
    Q_OBJECT
public:
    explicit QuCircularPlotW(QWidget *parent = nullptr);
    virtual  ~QuCircularPlotW();

private:
    QuCircularPlotW_P *d;


    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:

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
