#include "out_of_bounds_distortions.h"

double OOBDistort::x_distort(double x, const double &, const double &,
                                 const double &, const double &) const {
    return x;
}

double OOBDistort::y_distort(double y, const double &, const double &,
                                 const double &, const double &) const {
    return y;
}

void OOBDistort::bounds_distort(double *x, double *X, double *y, double *Y,
                                    const double &xlb, const double &xub, const double &ylb, const double &yub,
                                    const double &xmin, const double &xmax, const double &ymin, const double &ymax) const {
    double xx = (xmax - xmin) * 0.4;
    double yy = (ymax - ymin) * 0.4;
    *x = xlb <= xmin ? xlb : xlb - xx;
    *X = xub >= xmax ? xub : xub + xx;
    *y = ylb <= ymin ? ylb : ylb - yy;
    *Y = yub >= ymax ? yub : yub + yy;
}

double OOBLogDistort::x_distort(double x, const double &, const double &,
                                    const double &, const double &) const {
    return x;
}

double OOBLogDistort::y_distort(double y, const double &ylb, const double &yub,
                                    const double &ymin, const double &ymax) const {
    (void) ymin; (void) ymax;
    if(y < ylb) y = ylb - log(ylb - y);
    if(y > yub) y = yub + log(y - yub);
    return y;
}

void OOBLogDistort::bounds_distort(double *x, double *X, double *y, double *Y,
                                       const double &xlb, const double &xub, const double &ylb, const double &yub,
                                       const double &xmin, const double &xmax, const double &ymin, const double &ymax) const {
    double xx = (xmax - xmin) * 1.1; // x extra
    double yy = (ymax - ymin) * 1.1;
    double dY = ymax + yy;
    double dy = ymin - yy;
    double dX = xmax + xx;
    double dx = xmin - xx;
    *x = xlb <= xmin ? xlb : xlb - (dx > 0 ? log(dx) : log(-dx));
    *X = xub >= xmax ? xub : xub + (dX > 0 ? log(dX) : log(-dX));
    *y = ylb <= ymin ? ylb : ylb - (dy > 0 ? log(dy) : log(-dy));
    *Y = yub >= ymax ? yub : yub + (dY > 0 ? log(dY) : log(-dY));
}
