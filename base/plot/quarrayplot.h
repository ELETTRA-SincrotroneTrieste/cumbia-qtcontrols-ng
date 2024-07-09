#ifndef QUARRAYPLOT_H
#define QUARRAYPLOT_H

#include <qwt_plot.h>

class QuArrayPlotP;

class QuArrayPlot : public QwtPlot
{
public:
    QuArrayPlot(QWidget *parent, bool opengl = false);
    virtual ~QuArrayPlot();

private:
    QWidget *m_make_GL_canvas();
    QWidget *m_make_canvas();
    void m_align_scales();


    QuArrayPlotP *d;
};

#endif // QUARRAYPLOT_H
