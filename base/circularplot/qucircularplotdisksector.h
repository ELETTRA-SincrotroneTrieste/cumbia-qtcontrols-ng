#ifndef QUCIRCULARPLOTDISKSECTOR_H
#define QUCIRCULARPLOTDISKSECTOR_H

#include <qucircularplot_drawable_i.h>
#include <QStringList>
#include <QColor>

class QuCircularPlotDiskSector_P;
class QPointF;

class QuCircularPlotDiskSector : public QuCircularPlotDrawable_I
{
public:
    QuCircularPlotDiskSector();
    virtual ~QuCircularPlotDiskSector();

    void setSections(const QStringList& names);
    void setSectionColors(const QVector<QColor>& c);
    void setBackgroundColor(const QColor& c);
    void setTextDist(const float& f);

    QVector<QColor> sectionColors() const;
    QStringList sections() const;
    QColor backgroundColor() const;
    float textDist() const;

    int index(const QPointF& pt);

    void setDrawFrom(float from);
    void setDrawTo(float to);

    // QuCircularPlotDrawable_I interface
public:
    bool scales();
    int z() const;
    void draw(QPainter *p, const QuCircularPlotEngine *plot_e, double inner_radius, double outer_radius, const QRectF &rect, QWidget *widget);

private:
    QuCircularPlotDiskSector_P *d;
};

#endif // QUCIRCULARPLOTDISKSECTOR_H
