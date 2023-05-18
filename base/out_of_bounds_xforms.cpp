#include "out_of_bounds_xforms.h"

double OOBTransform::x_transform(double x, const double &, const double &,
                                 const double &, const double &) const {
    return x;
}

double OOBTransform::y_transform(double y, const double &ylb, const double &yub,
                                 const double &ymin, const double &ymax) const {
    return y;
}

void OOBTransform::bounds_transform(double *x, double *X, double *y, double *Y,
                                    const double &xlb, const double &xub, const double &ylb, const double &yub,
                                    const double &xmin, const double &xmax, const double &ymin, const double &ymax) const {
    *x = xlb <= xmin ? xlb : xmin;
    *X = xub >= xmax ? xub : xmax;
    *y = ylb <= ymin ? ylb : ymin;
    *Y = yub >= ymax ? yub : ymax;
}

double OOBLogTransform::x_transform(double x, const double &, const double &,
                                    const double &, const double &) const {
    return x;
}

double OOBLogTransform::y_transform(double y, const double &ylb, const double &yub,
                                    const double &ymin, const double &ymax) const
{
    if(y < ylb) y = ylb - log(ylb - y);
    if(y > yub) y = yub + log(y - yub);
    return y;
}

void OOBLogTransform::bounds_transform(double *x, double *X, double *y, double *Y,
                                       const double &xlb, const double &xub, const double &ylb, const double &yub,
                                       const double &xmin, const double &xmax, const double &ymin, const double &ymax) const {
    *x = xlb <= xmin ? xlb : (xmin > 0 ? log(xmin) : -log(-xmin));
    *X = xub >= xmax ? xub : (xmax > 0 ? log(xmax) : -log(-xmax));
    *y = ylb <= ymin ? ylb : (ymin > 0 ? log(ymin) : -log(-ymin));
    *Y = yub >= ymax ? yub : (ymax > 0 ? yub + log(ymax - yub) : yub + log(yub-ymax));
}
