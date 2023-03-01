#ifndef QUNUMERICENGINE_H
#define QUNUMERICENGINE_H

#include <QFont>
#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QVector>

class QStyleOptionGraphicsItem;
class QPainter;

class QuANRect : public QRectF {
public:
    enum Type { Sign = -1000010, Dot = -1000009, Apply = -1000008, Invalid = -1, Digit = 0 }; // > 0 index

    QuANRect(double x, double y, double w, double h, int pos) : QRectF(x, y, w, h), pos(pos) { }

    int pos;
};

class QuApplyNumEngineData {
public:
    int intDig;
    int decDig;
    int digits;
    long long data;
    long long last_applied_data;
    long long min;
    long long max;

    double min_d, max_d;
    double font_scale;

    // margins
    double lm, bm, rm, um;
    // arrow height
    double arrow_hei;
    QSizeF minsiz;
    QVector<QuANRect> digitrects;
    QFont font;
    QPointF mouse_pos;
    bool mouse_down;
    bool show_apply;

    int maxcharw;
};

class QuNumericEngine : public QObject
{
    Q_OBJECT
public:
    QuApplyNumEngineData d;

    explicit QuNumericEngine(const QFont &f);
    virtual  ~QuNumericEngine();

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

    QSizeF minimumSize();

public slots:
    void setValue(double v);
    void setMaximum(double m);
    void setMinimum(double m);

    void setIntDigits(int i);
    void setDecDigits(int d);

    void click(const QPointF& pos);
    void click(const QPointF& pos, bool up);

    void setShowApply(bool show);

signals:
    void valueChanged(double v);
    void minimumChanged(double m);
    void maximumChanged(double M);
    void intDigitsChanged(int id);
    void decDigitsChanged(int dd);

    void applyClicked(double val);

private:

    void m_drawHighlighted();

    int m_map_to_digit_pos(const QPointF& mousePos) const;
    int m_digit_index(int index) const;
    bool m_up(const QPointF& mousePos) const;
    void m_update_bounds();

    double m_to_double(long long lli) const;

    void m_apply_clicked();

    void m_update_maxcharw();

    QuANRect::Type m_rect_type(int idx) const;

private slots:

};

#endif // QUNUMERICENGINE_H
