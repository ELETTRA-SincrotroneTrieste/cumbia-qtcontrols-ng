#ifndef CUAPPLYNUMERICENGINE_H
#define CUAPPLYNUMERICENGINE_H

#include <QObject>

class CuApplyNumericEnginePrivate;

class CuApplyNumericEngine : public QObject
{
    Q_OBJECT
public:
    explicit CuApplyNumericEngine(QObject *parent = nullptr);
    virtual  ~CuApplyNumericEngine();

    void contextMenuEvent(const QPointF& pos);
    void mousePressEvent(const QPointF& pos);
    void mouseMoveEvent(const QPointF& pos);
    void mouseReleaseEvent(const QPointF& pos);
    void mouseDoubleClickEvent(const QPointF& pos);
    void wheelEvent(const QPointF& pos, double delta);

signals:

private:
    CuApplyNumericEnginePrivate *d;
};

#endif // CUAPPLYNUMERICENGINE_H
