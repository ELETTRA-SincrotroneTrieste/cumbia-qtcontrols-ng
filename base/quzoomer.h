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
    bool eventFilter(QObject *watched, QEvent *event);

    QPointF p1, p2;

    QRectF zoomRect() const;

    bool inZoom() const;

    int stackSize() const;

    bool drawZoomRect(QPainter *p, const QRectF &r = QRectF(), QWidget *widget = nullptr);
    bool zoom(QPainter *p, const QRectF &r = QRectF(), QWidget *widget = nullptr);

signals:

private:
    QuZoomerP *d;

    void m_update(QObject *o);

    void m_move_zooms();

    void m_add_zoom(const QRectF &area);

    QPointF m_map_from_pos(const QPointF& pt);

    QPainter m_get_painter(QObject *o) const;
};

#endif // QUZOOMER_H
