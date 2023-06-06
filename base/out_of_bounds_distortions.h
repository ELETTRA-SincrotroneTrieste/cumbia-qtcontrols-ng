#ifndef OOBTRANSFORM_H
#define OOBTRANSFORM_H

#include <math.h>

/*!
 * \brief The OOBDistort class linearly distorts (no distortion) x and y when out of bounds.
 */
class OOBDistort {
public:
    virtual ~OOBDistort() {}

    virtual double x_distort(double v, const double &xlb, const double& xub,
                               const double& xmin, const double& xmax) const;
    virtual double y_distort(double v, const double &ylb, const double& yub,
                               const double& ymin, const double& ymax) const;
    virtual void bounds_distort(double *x, double *X, double *y, double *Y, const double &xlb, const double &xub, const double &ylb, const double &yub, const double &xmin, const double &xmax, const double &ymin, const double &ymax) const;
};

/*!
 * \brief Applies a logarithmic distortion to x and y values when they are out of bounds
 */
class OOBLogDistort : public OOBDistort {
public:
    OOBLogDistort(float base = M_E) { m_base = base; }

    virtual ~OOBLogDistort() {}

    virtual double x_distort(double v, const double &xlb, const double& xub,
                               const double& xmin, const double& xmax) const;
    virtual double y_distort(double v, const double &ylb, const double& yub,
                               const double& ymin, const double& ymax) const;
    virtual void bounds_distort(double *x, double *X, double *y, double *Y, const double &xlb, const double &xub, const double &ylb, const double &yub, const double &xmin, const double &xmax, const double &ymin, const double &ymax) const;

    float m_base;
};


#endif // OOBTRANSFORM_H
