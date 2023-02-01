#ifndef CUAPPLYNUMERICENGINE_H
#define CUAPPLYNUMERICENGINE_H

#include <QObject>

class QStyleOptionGraphicsItem;
class QPainter;


class CuApplyNumEngineData {
public:
    int intDig;
    int decDig;
    int digits;
    long long data;
    long long minVal;
    long long maxVal;

    double d_minAsDouble, d_maxAsDouble;
};

class CuApplyNumericEngine : public QObject
{
    Q_OBJECT
public:
    CuApplyNumEngineData d;

    explicit CuApplyNumericEngine(QObject *parent = nullptr);
    virtual  ~CuApplyNumericEngine();

    void contextMenuEvent(const QPointF& pos);
    void mousePressEvent(const QPointF& pos);
    void mouseMoveEvent(const QPointF& pos);
    void mouseReleaseEvent(const QPointF& pos);
    void mouseDoubleClickEvent(const QPointF& pos);
    void wheelEvent(const QPointF& pos, double delta);

    void paint(QPainter *p, const QRectF &rect, QWidget *widget);

signals:

private:
};

#endif // CUAPPLYNUMERICENGINE_H
