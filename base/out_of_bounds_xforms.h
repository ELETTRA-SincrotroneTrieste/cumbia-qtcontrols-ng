#ifndef OOBTRANSFORM_H
#define OOBTRANSFORM_H

#include <math.h>

class OOBTransform {
public:
    virtual ~OOBTransform() {}

    virtual double x_transform(double v, const double &xlb, const double& xub,
                               const double& xmin, const double& xmax) const;
    virtual double y_transform(double v, const double &ylb, const double& yub,
                               const double& ymin, const double& ymax) const;
    virtual void bounds_transform(double *x, double *X, double *y, double *Y, const double &xlb, const double &xub, const double &ylb, const double &yub, const double &xmin, const double &xmax, const double &ymin, const double &ymax) const;
};

class OOBLogTransform : public OOBTransform {
public:
    OOBLogTransform(float base = M_E) { m_base = base; }

    virtual ~OOBLogTransform() {}

    virtual double x_transform(double v, const double &xlb, const double& xub,
                               const double& xmin, const double& xmax) const;
    virtual double y_transform(double v, const double &ylb, const double& yub,
                               const double& ymin, const double& ymax) const;
    virtual void bounds_transform(double *x, double *X, double *y, double *Y, const double &xlb, const double &xub, const double &ylb, const double &yub, const double &xmin, const double &xmax, const double &ymin, const double &ymax) const;

    float m_base;
};


#endif // OOBTRANSFORM_H
