#ifndef LINEPAINTERPRIVATE_H
#define LINEPAINTERPRIVATE_H

#include <QPen>
#include <QBrush>

class CurveItem;

class LinePainterPrivate
{
public:
    LinePainterPrivate() : curveItem(nullptr), draw_baseline(true) {}

    QPen pen;
    QBrush brush;

    CurveItem* curveItem;

    bool draw_baseline; // default: true. needs a valid brush
};

#endif // LINEPAINTERPRIVATE_H
