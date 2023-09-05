#ifndef DOTSPAINTERPRIVATE_H
#define DOTSPAINTERPRIVATE_H

#include <QPen>

class CurveItem;

class DotsPainterPrivate
{
public:
    QPen pen;

    QBrush brush;

    double radius;

    CurveItem* curveItem;
};

#endif // LINEPAINTERPRIVATE_H
