#ifndef QUSCALARPLOTGL_H
#define QUSCALARPLOTGL_H

#include "quscalarplot.h"

/*!
 * \brief OpenGL engine flavour of QuScalarPlot, same interface
 *
 * See QuArrayPlotGL for further information
 *
 * \see QuScalarPlot
 * \see QuPlotOptions
 */
class QuScalarPlotGL : public QuScalarPlot
{
    Q_OBJECT
public:
    QuScalarPlotGL(QWidget *parent);
};

#endif // QUSCALARPLOTGL_H
