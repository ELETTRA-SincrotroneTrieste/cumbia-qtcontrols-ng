#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void setProp(double v);
    void setProp(int i);

    void onMinChanged(double m);
    void onMaxChanged(double M);
    void onDecDigitsChanged(int dd);
};
#endif // WIDGET_H
