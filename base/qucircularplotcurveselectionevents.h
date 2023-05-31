#ifndef QUCIRCULARPLOTCURVESELECTIONEVENTS_H
#define QUCIRCULARPLOTCURVESELECTIONEVENTS_H

#include <QObject>

class QRectF;
class QuCircularPlotCurveSelectionEvents_P;

class QuCircularPlotCurveSelectionEvents : public QObject
{
    Q_OBJECT
public:
    explicit QuCircularPlotCurveSelectionEvents(QObject *parent = nullptr);
    virtual ~QuCircularPlotCurveSelectionEvents();

    bool eventFilter(QObject *watched, QEvent *event);

    void select(const QRectF& r);
    const QRectF &selectionArea() const;

public slots:
    void setTransform(const QTransform& T);

signals:
    void selected(const QPointF &p);
    void selectionMoved(const QPointF& p);
    void deselected();

private:
    QuCircularPlotCurveSelectionEvents_P *d;

};

#endif // QUCIRCULARPLOTCURVESELECTIONEVENTS_H
