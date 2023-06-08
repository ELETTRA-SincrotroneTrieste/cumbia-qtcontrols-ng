#ifndef QUCIRCULARPLOTCURVE_H
#define QUCIRCULARPLOTCURVE_H

#include <QVector>
#include <QPen>

class QuCircularPlotCurveP;

class QuCircularPlotCurveListener {
public:
    virtual void onCurveBoundsChanged() = 0;
};

class QuCircularPlotCurve
{
public:
    QuCircularPlotCurve(const QString &src, QuCircularPlotCurveListener *l, const QPen& p = QPen(QColor(Qt::gray), 0.0f));
    virtual ~QuCircularPlotCurve();

    const QVector<double> &x_data() const;
    const QVector<double> &y_data() const;

    QString source() const;
    void setData(const QVector<double> &x, const QVector<double>& y);
    int size() const;

    void setPen(const QPen& p);
    QPen pen() const;

    double ymax() const;
    double ymin() const;

    void minmax_update();

    // selection
    void select(int idx);
    void deselect();
    int selected() const;
    // editable
    bool isEditable() const;
    void setEditable(bool e);

private:
    QuCircularPlotCurveP *d;
};

#endif // QUCIRCULARPLOTCURVE_H
