#include "qgraphicsplotitem_private.h"

QGraphicsPlotItemPrivate::QGraphicsPlotItemPrivate() {
    scaleOnScroll = true;
    scaleOnResize = true;
    neverShown = true;
    modifiedPaintEvent = false;
    mouseZoomEnabled = false;
    sceneRectToWidgetGeometry = true;
    mousePressed  = mouseMoving = false;
    topLeftXPercent = topLeftYPercent = 0.0;
    widthPercent = heightPercent = 1.0;
    firstScrollM11  = firstScrollM12 = -1.0;
    backgroundColor = QColor();
    useGl = false;
    menu = nullptr;
}
