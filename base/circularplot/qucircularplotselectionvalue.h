#ifndef QUCIRCULARPLOTSELECTIONVALUE_H
#define QUCIRCULARPLOTSELECTIONVALUE_H

#include <QObject>
#include <qucircularplot_drawable_i.h>
#include <QRectF>
#include <QFont>

class QuCircularPlotCurve;

class QuCircularPlotSelectionValue : public QObject, public QuCircularPlotDrawable_I {
    Q_OBJECT
public:
    QuCircularPlotSelectionValue(QObject *parent);

public slots:
    void update(QuCircularPlotCurve *c);

    // QuCircularPlotDrawable_I interface
public:
    bool scales();
    int z() const;
    void draw(QPainter *p, const QuCircularPlotEngine *plot_e, double inner_radius, double outer_radius, const QRectF &rect, QWidget *widget);

private:
    int m_i, m_v;
    bool m_v_changed;
    QRectF m_rect;
    QFont m_font;
};

#endif // QUCIRCULARPLOTSELECTIONVALUE_H
