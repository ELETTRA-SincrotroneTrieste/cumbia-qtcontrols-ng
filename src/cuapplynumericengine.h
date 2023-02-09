#ifndef CUAPPLYNUMERICENGINE_H
#define CUAPPLYNUMERICENGINE_H

#include <QFont>
#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QRectF>

class QStyleOptionGraphicsItem;
class QPainter;

class CuANRect : public QRectF {
public:
    CuANRect(double x, double y, double w, double h, int pos) : QRectF(x, y, w, h), pos(pos) { }

    int pos;
};

class CuApplyNumEngineData {
public:
    int intDig;
    int decDig;
    int digits;
    long long data;
    long long min;
    long long max;

    double min_d, max_d;
    double font_scale;

    // margins
    double lm, bm, rm, um;
    // arrow height
    double arrow_hei;
    QSizeF minsiz;
    QVector<CuANRect> digitrects;
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

    double value() const;
    double minimum() const;
    double maximum() const;

    void paint(QPainter *p, const QRectF &rect, QWidget *widget);
    void setFont(const QFont& f);

    QSizeF minimumSize() const;

public slots:
    void setValue(double v);
    void setMaximum(double m);
    void setMinimum(double m);

    void setIntDigits(int i);
    void setDecDigits(int d);

    void step(const QPointF& pos);

signals:
    void valueChanged(double v);
    void minimumChanged(double m);
    void maximumChanged(double M);
    void intDigitsChanged(int id);
    void decDigitsChanged(int dd);

private:

    void m_drawHighlighted();

    int m_map_to_digit_pos(const QPointF& mousePos) const;
    int m_digit_index(int index) const;
    bool m_up(const QPointF& mousePos) const;
    void m_update_bounds();

    double m_to_double(long long lli) const;

private slots:

};

#endif // CUAPPLYNUMERICENGINE_H
