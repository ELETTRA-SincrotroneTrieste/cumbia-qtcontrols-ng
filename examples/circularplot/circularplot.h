#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QuCircularPlotCurve;

class CircularPlot : public QWidget
{
    Q_OBJECT

public:
    CircularPlot(QWidget *parent = nullptr);
    ~CircularPlot();

private slots:
    void update();
    void editCurve(QuCircularPlotCurve *curve, int idx, double ynew);
    void editCurve(QObject *plot, const QString& curve, int idx, double ynew);


private:
    Ui::Widget *ui;

    int ncurves;
    int timeo;

};
#endif // WIDGET_H
