#ifndef CUAPPLYNUMERICITEM_H
#define CUAPPLYNUMERICITEM_H

#include <QGraphicsObject>

class CuApplyNumericItemPrivate;

class CuApplyNumericItem : public QGraphicsObject
{
    Q_OBJECT
public:
    CuApplyNumericItem();
    virtual ~CuApplyNumericItem();

private:
    CuApplyNumericItemPrivate *d;
};

#endif // CUAPPLYNUMERICITEM_H
