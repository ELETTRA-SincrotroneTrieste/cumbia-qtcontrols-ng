#ifndef QUARRAYPLOT_H
#define QUARRAYPLOT_H

#include <qwt_plot.h>
#include <vector>
#include <QPen>

class QuArrayPlotP;
class QwtPlotCurve;

class QuArrayPlot : public QwtPlot
{
public:
    QuArrayPlot(QWidget *parent, bool opengl = false);
    virtual ~QuArrayPlot();

    QwtPlotCurve *addCurve(const std::string& name, const QPen& pen = QPen());
    virtual void setData(const std::string &name, const std::vector<double>& y);
    virtual void onError(const std::string&name, const std::string& msg);

private:
    QWidget *m_make_GL_canvas();
    QWidget *m_make_canvas();
    void m_align_scales();


    QuArrayPlotP *d;
};

#endif // QUARRAYPLOT_H
