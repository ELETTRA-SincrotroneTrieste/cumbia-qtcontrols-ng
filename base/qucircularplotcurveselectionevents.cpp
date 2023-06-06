#include "qucircularplotcurveselectionevents.h"
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QRectF>
#include <QTransform>
#include <QtDebug>

class  QuCircularPlotCurveSelectionEvents_P {
public:
    QuCircularPlotCurveSelectionEvents_P() : press_in_selection(false) {}

    QRectF sel;
    QPointF press_pt, move_pt;
    bool press_in_selection;
    QTransform T;
};

QuCircularPlotCurveSelectionEvents::QuCircularPlotCurveSelectionEvents(QObject *parent)
    : QObject{parent}, d(new QuCircularPlotCurveSelectionEvents_P) {
}

QuCircularPlotCurveSelectionEvents::~QuCircularPlotCurveSelectionEvents() {
    delete d;
}

bool QuCircularPlotCurveSelectionEvents::eventFilter(QObject *watched, QEvent *event) {
    bool ret = false;
    Qt::MouseButton butt;
    Qt::KeyboardModifiers m;
    bool graphicss = event->type() == QEvent::GraphicsSceneMousePress ||
                     event->type() == QEvent::GraphicsSceneMouseRelease ||
                     event->type() == QEvent::GraphicsSceneMouseMove ||
                     event->type() == QEvent::GraphicsSceneHoverMove;
    bool press = ( (event->type() == QEvent::GraphicsSceneMousePress) || (event->type() == QEvent::MouseButtonPress));
    bool release = ((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::MouseButtonRelease));
    bool move = ((event->type() == QEvent::GraphicsSceneHoverMove) || (event->type() == QEvent::MouseMove));
    bool scene_move = (event->type() == QEvent::GraphicsSceneMouseMove);
    QPointF pos;
    const QTransform& Ti = d->T.inverted();
    if(press || release || move || scene_move) {
        if(graphicss) {
            QGraphicsSceneMouseEvent *sme = static_cast<QGraphicsSceneMouseEvent *>(event);
            butt = sme->button();
            m = sme->modifiers();
            pos = /*Ti.map*/(sme->pos());
        }
        else {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            butt = me->button();
            m = me->modifiers();
            pos = me->pos();
        }
        if(scene_move && d->press_in_selection) {
            ret = true;
        }
        else if(scene_move) {
            d->press_pt = QPointF();
        }
        if(press) {
            d->press_pt = pos;
            d->press_in_selection = d->sel.contains(Ti.map(pos));
        }
        else if((move || scene_move) && d->press_in_selection) {
            printf("QuCircularPlotCurveSelectionEvents::eventFilter \e[1;34m HOVER move event and selection contains\e[0m\n");
            qDebug() << "pos is " << pos << "mapped is ---------------->> " << Ti.map(pos);
            const QPointF& old_p = d->move_pt.isNull() ? d->press_pt : d->move_pt;
            emit selectionMoved(pos.rx() - old_p.x(), pos.ry() - old_p.y());
            d->move_pt = pos;
            ret = true;
        }
        else if(release && pos == d->press_pt && butt == Qt::LeftButton) {
            emit selected(d->press_pt);
            d->press_pt = d->move_pt = QPointF();
        }
        else if(release && butt == Qt::MiddleButton && d->sel.isValid()) {
            d->sel = QRectF();
            emit deselected();
        }

        if(release) {
            d->press_pt = d->move_pt =QPointF();
            d->press_in_selection = false;
        }

    }
    return ret ? ret : QObject::eventFilter(watched, event);
}

void QuCircularPlotCurveSelectionEvents::select(const QRectF &r) {
    d->sel = r;
}

const QRectF &QuCircularPlotCurveSelectionEvents::selectionArea() const {
    return d->sel;
}

void QuCircularPlotCurveSelectionEvents::setTransform(const QTransform &T) {
    d->T = T;
}
