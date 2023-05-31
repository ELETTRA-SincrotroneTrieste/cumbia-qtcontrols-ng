#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class CircularPlot : public QWidget
{
    Q_OBJECT

public:
    CircularPlot(QWidget *parent = nullptr);
    ~CircularPlot();

private slots:
    void update();

private:
    Ui::Widget *ui;

    int ncurves;
    int timeo;

};
#endif // WIDGET_H
