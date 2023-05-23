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
    const QRect& setViewport(QPainter *painter, const QRectF &r) const;

public slots:
    void setZoomRect(const QRectF& r);
    void moveZoom(const QPointF& p1, const QPointF& p2);
    void unzoom();
    void zoomRectChanging(const QPointF& topl, const QPointF& botr);

    void sizeChanged(const QSize& newSiz);


signals:
    void zoomChanged(); // update widget

private:
    QuZoomerP *d;

    QPointF m_map_from_pos(const QPointF &pt);
    void m_add_zoom(const QRectF &area);
    QPainter m_get_painter(QObject *o) const;
};

#endif // QUZOOMER_H
