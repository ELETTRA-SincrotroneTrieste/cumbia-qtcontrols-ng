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
