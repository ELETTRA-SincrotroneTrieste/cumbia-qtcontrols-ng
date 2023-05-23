#ifndef QUZOOMEVENTS_H
#define QUZOOMEVENTS_H

#include <QObject>
#include <QPointF>

class QuZoomEventsP;

class QuZoomEvents : public QObject
{
    Q_OBJECT
public:
    enum Operation { Zoom = 0, Move, EndOps };
    explicit QuZoomEvents(QObject *parent);
    virtual ~QuZoomEvents();

    QPointF p1, p2;

    void setKeyboardModifier(Operation op, Qt::KeyboardModifier mod);
    Qt::KeyboardModifier keyboardModifier(Operation op) const;

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void zoomRectChanged(const QRectF& r);
    void unzoom();
    void moveRect(const QPointF& p1, const QPointF& p2);
    void zoomRectChanging(const QPointF &topl, const QPointF br);

private:
    QuZoomEventsP *d;

};

#endif // QUZOOMEVENTS_H
