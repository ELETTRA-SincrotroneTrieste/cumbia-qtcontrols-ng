#include "scaleitemprivate.h"
#include <math.h>

ScaleItemPrivate::ScaleItemPrivate()
{
    orientation = ScaleItem::Horizontal;
    tickStepLen = actualTickStepLen = -1;
    tickStepLenScale = 1.0;
    plotZoomLevel = 1;

    labelsEnabled  = true;
    axisLabelRotation = 90;

    fontSize = 8.0;

    mLastTickPos = 0; /* origin */

    axisLabelsFormat = "";

    gridEnabled = ticksEnabled = true;
    gridColor = QColor(218, 221, 224); /* KLIGHTGRAY */
    axisColor = Qt::black;
    axisTitleColor = QColor(9, 13, 21); /* KALMOSTBLACK */

    upperBound = 1000;
    lowerBound = -1000;
    xlbMargin = 0.01; /* 2 % */
    xubMargin = 0.05;
    ylbMargin = yubMargin = 0.01;

    autoScale = enabled = true;
    minMaxUnset = true;
}
