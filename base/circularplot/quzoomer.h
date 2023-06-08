#ifndef QUZOOMER_H
#define QUZOOMER_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QRectF>
#include <QPainter>

class QStyleOptionGraphicsItem;
class QWidget;

class QuZoomerP;

class QuZoomer : public QObject
{
    Q_OBJECT
public:
    explicit QuZoomer(QObject *parent);
    virtual ~QuZoomer();

    QRectF zoomRect() const;
    const QRectF &zoomArea() const; // to draw the zoom rectangle
    bool inZoom() const;
    int stackSize() const;
    bool rectChanging() const;
    bool viewportChanged() const;
    const QTransform& calculateTransform(const QRectF &r) const;
    const QTransform & transform() const;
    QPointF map(const QPointF &pt);

public slots:
    void setZoomRect(const QRectF& r);
    void moveZoom(const QPointF& p1, const QPointF& p2);
    void unzoom();
    void zoomRectChanging(const QPointF& topl, const QPointF& botr);
    void mapClick(const QPointF& p, const Qt::MouseButton butt, Qt::KeyboardModifiers mod);


signals:
    void zoomChanged(); // update widget
    void moved(double dx, double dy);
    void clicked(const QPointF& p, const Qt::MouseButton butt, Qt::KeyboardModifiers mod);

private:
    QuZoomerP *d;

    void m_add_zoom(const QRectF &area);
    QPainter m_get_painter(QObject *o) const;
};

#endif // QUZOOMER_H
