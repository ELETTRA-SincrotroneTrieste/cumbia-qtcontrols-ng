#include "cuapplynumericengine.h"

class CuApplyNumericEnginePrivate {
public:
};

CuApplyNumericEngine::CuApplyNumericEngine(QObject *parent)
    : QObject{parent} {
    d = new CuApplyNumericEnginePrivate;
}

CuApplyNumericEngine::~CuApplyNumericEngine() {
    delete d;
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
