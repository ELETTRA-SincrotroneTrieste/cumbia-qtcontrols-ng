#include "cuapplynumericitem.h"

class CuApplyNumericItemPrivate {
public:
};

CuApplyNumericItem::CuApplyNumericItem() {
    d = new CuApplyNumericItemPrivate;
}

CuApplyNumericItem::~CuApplyNumericItem() {
    delete d;
}
