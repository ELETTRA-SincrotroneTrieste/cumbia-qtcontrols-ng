#ifndef QUZOOMER_H
#define QUZOOMER_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QRectF>

class QuZoomer : public QObject
{
    Q_OBJECT
public:
    explicit QuZoomer(QObject *parent);

    bool eventFilter(QObject *watched, QEvent *event);

    QPointF p1, p2;

    QRectF zoomRect() const;

    bool inZoom() const;

    int stackSize() const;

signals:

private:
    QVector<QRectF> m_zstack;

    void m_update(QObject *o);
};

#endif // QUZOOMER_H
