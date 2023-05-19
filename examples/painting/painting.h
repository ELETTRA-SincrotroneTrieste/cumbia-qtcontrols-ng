#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QuZoomer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

    ~Widget();

    QuZoomer *m_zoomer;
};
#endif // WIDGET_H
