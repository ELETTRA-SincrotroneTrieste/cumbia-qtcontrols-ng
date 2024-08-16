#ifndef QUARRAYPLOTGL_H
#define QUARRAYPLOTGL_H

#include "quarrayplot.h"

/*!
 * \brief The openGL version of QuArrayPlot
 *
 * Same interface as QuArrayPlot, internally using
 * QwtPlotOpenGLCanvas instead of QwtPlotCanvas
 *
 * You can change the engine at runtime through QuPlotOptions
 *
 * \see QuArrayPlot
 * \see QuPlotOptions
 */
class QuArrayPlotGL : public QuArrayPlot
{
public:
    QuArrayPlotGL(QWidget *parent);
};

#endif // QUARRAYPLOTGL_H
