#include "quarraybuf.h"
#include "quarrayplot.h"
#include "qucurves.h"
#include <qwt_plot_opengl_canvas.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_engine.h>
#include <cumacros.h>

class QuArrayPlotP {
public:
    QuArrayPlotP(bool ogl) : opengl(ogl), grid(nullptr) {}

    bool opengl;
    QwtPlotGrid *grid;
    QuCurves *curves;
};

QuArrayPlot::QuArrayPlot(QWidget *parent, bool opengl) : QwtPlot(parent) {
    d = new QuArrayPlotP(opengl);
    d->curves = new QuCurves();
    if(opengl) {
        setCanvas( m_make_GL_canvas() );
    } else {
        setCanvas(m_make_canvas());
    }
    m_align_scales();
    int axes[4]{ xBottom, xTop, yLeft, yRight};
    for(int axisId : axes )
        axisScaleEngine(axisId)->setAttribute(QwtScaleEngine::Floating);
    // Insert grid
    d->grid = new QwtPlotGrid();
    d->grid->attach( this );
}

QuArrayPlot::~QuArrayPlot() {
    delete d->curves;
    delete d;
}

QwtPlotCurve *QuArrayPlot::addCurve(const std::string &name,
                                    QwtAxisId xAxis,
                                    QwtAxisId yAxis,
                                    const QPen &pen) {
    QwtPlotCurve *c = nullptr;
    if(!d->curves->map.contains(name)) {
        // get a new curve with a default color if not specified
        c = d->curves->get(name, pen);
        c->attach(this);
        c->setAxes(xAxis, yAxis);
        bool xa = axisAutoScale(xAxis), ya = axisAutoScale(yAxis);
        QuArrayBuf* b = new QuArrayBuf(xa, ya);
        c->setData(b);
        pretty_pri("addCurve on curve %p with buf %p (data() returns buf %p)",
                   c, b, static_cast<QuArrayBuf *>(c->data()));
    }
    emit curveAdded(c);
    return c;
}

void QuArrayPlot::setData(const std::string& name, const std::vector<double> &y) {
    QwtPlotCurve *c = d->curves->get(name);
    if(c) {
        QuArrayBuf *buf = static_cast<QuArrayBuf *>(c->data());
        // pretty_pri("%s -> data siz %ld: y:[%.1f - %.1f] curve %p buf %p plot autoscale x %s, y %s",
        //            name.c_str(), y.size(), buf->o.ylb, buf->o.yub, c, buf,
        //            axisAutoScale(QwtPlot::xBottom) ? "YES" : "NO",
        //            axisAutoScale(QwtPlot::yLeft) ? "YES": "NO");

        buf->move(y);
        replot();
        emit dataUpdated(c);
    }
}

void QuArrayPlot::onError(const std::string &name, const std::string &msg) {
    emit error(name, msg);
}

//
//  Set a plain canvas frame and align the scales to it
//  Taken from Qwt examples/refreshtest
//
void QuArrayPlot::m_align_scales() {
    // The code below shows how to align the scales to
    // the canvas frame, but is also a good example demonstrating
    // why the spreaded API needs polishing.
    for ( int axisPos = 0; axisPos < QwtAxis::AxisPositions; axisPos++ ) {
        QwtScaleWidget* scaleWidget = axisWidget( axisPos );
        if ( scaleWidget )
            scaleWidget->setMargin( 0 );

        QwtScaleDraw* scaleDraw = axisScaleDraw( axisPos );
        if ( scaleDraw )
            scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
    }

    plotLayout()->setAlignCanvasToScales( true );
}

QWidget *QuArrayPlot::m_make_GL_canvas() {
    QwtPlotOpenGLCanvas* canvas = new QwtPlotOpenGLCanvas();
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setLineWidth( 1 );
    canvas->setPalette( Qt::white );
    return canvas;
}

QWidget *QuArrayPlot::m_make_canvas() {
    QwtPlotCanvas* canvas = new QwtPlotCanvas();
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setLineWidth( 1 );
    canvas->setPalette(Qt::white);
    return canvas;
}
