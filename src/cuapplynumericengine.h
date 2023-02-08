#ifndef CUAPPLYNUMERICENGINE_H
#define CUAPPLYNUMERICENGINE_H

#include <QFont>
#include <QObject>
#include <QPointF>
#include <QSizeF>

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
    double font_scale;

    // margins
    double lm, bm, rm, um;
    // arrow height
    double arrow_hei;
    QSizeF minsiz;
    QList<QRectF> digitrects;
    QFont font;
    QPointF mouse_pos;
    bool mouse_down;
};

class CuApplyNumericEngine : public QObject
{
    Q_OBJECT
public:
    CuApplyNumEngineData d;

    explicit CuApplyNumericEngine(const QFont &f);
    virtual  ~CuApplyNumericEngine();

    void contextMenuEvent(const QPointF& pos);
    void mousePressEvent(const QPointF& pos);
    void mouseMoveEvent(const QPointF& pos);
    void mouseReleaseEvent(const QPointF& pos);
    void mouseDoubleClickEvent(const QPointF& pos);
    void wheelEvent(const QPointF& pos, double delta);

    void paint(QPainter *p, const QRectF &rect, QWidget *widget);

    void setFont(const QFont& f);

    QSizeF minimumSize() const;


signals:

private:
    void m_drawHighlighted();
};

#endif // CUAPPLYNUMERICENGINE_H
