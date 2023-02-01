#include "cuapplynumericengine.h"

#include <QPainter>



CuApplyNumericEngine::CuApplyNumericEngine(QObject *parent)
    : QObject{parent} {
}

CuApplyNumericEngine::~CuApplyNumericEngine() {

}

void CuApplyNumericEngine::contextMenuEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::mousePressEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::mouseMoveEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::mouseReleaseEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::mouseDoubleClickEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::wheelEvent(const QPointF &pos, double delta)
{

}

void CuApplyNumericEngine::paint(QPainter *p, const QRectF &rect, QWidget *widget) {
    QPen pen(Qt::green);
    p->setPen(pen);
    p->drawRect(rect);
}
