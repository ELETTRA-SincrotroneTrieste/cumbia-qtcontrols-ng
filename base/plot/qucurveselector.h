#ifndef QUCURVESELECTOR_H
#define QUCURVESELECTOR_H

#include "qpoint.h"
#include <cumacros.h>
#include <qwt_plot_curve.h>
#include <QObject>
#include <QList>
#include <QPen>

class QwtPlot;

class QuCurveSelectorP;

class QuCurveSelectionData {
public:
    QuCurveSelectionData(QwtPlotCurve *c, int i, float pw) : curve(c), idx(i), penw(pw) {}

    bool operator==(const QuCurveSelectionData &other) const {
        return other.curve == this->curve;
    }

    QwtPlotCurve *curve;
    int idx;
    float penw;
};

class QuCurveSelection {
public:
    QVector<QuCurveSelectionData> data;
    QPointF pos;

    int size() const { return data.size(); }

    inline int indexOf(const QwtPlotCurve *c) {
        for(int i = 0; i < data.size(); i++)
            if(data[i].curve == c)
                return i;
        return -1;
    }

    void add(QwtPlotCurve* c, int idx) {
        data.append(QuCurveSelectionData(c, idx, c->pen().widthF()));
    }

    bool isEmpty() {
        return data.size() == 0;
    }

    void clear() {
        data.clear();
        pos = QPointF();
    }

    bool operator==(const QuCurveSelection &other) const {
        if(other.data.size() != data.size())
            return false;
        return other.data == this->data;
    }
};

class QuCurveSelector : public QObject
{
    Q_OBJECT
public:
    explicit QuCurveSelector(QwtPlot *plot, bool highlight_point = true, bool highlight_curve = true);
    virtual ~QuCurveSelector();

    bool eventFilter(QObject *o, QEvent *e);

    QuCurveSelection selection() const;

    QwtPlot *plot() const;
    QList<QwtPlotCurve *> curves() const;

public slots:
    void setHightlightSelectedCurve(bool hl);
    void setMarkerEnabled(bool m);
    void setMarkerAutoUpdate(bool f);

    void select(const QPoint& pos);
    void deselect();

private slots:
    void dataUpdated(QwtPlotCurve *c);

signals:
    void selectionChanged();

private:
    bool m_highlight_selected_c(bool h);
    bool m_set_marker(bool m);
    QuCurveSelectorP* d;
};

#endif // QUCURVESELECTOR_H
