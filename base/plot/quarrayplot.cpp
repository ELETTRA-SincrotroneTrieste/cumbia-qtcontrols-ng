#include "quarrayplot.h"
#include <qwt_plot_opengl_canvas.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>

class QuArrayPlotP {
public:
    QuArrayPlotP(bool ogl) : opengl(ogl), grid(nullptr) {}

    bool opengl;
    QwtPlotGrid *grid;
};

QuArrayPlot::QuArrayPlot(QWidget *parent, bool opengl) : QwtPlot(parent) {
    d = new QuArrayPlotP(opengl);

    if(opengl) {
        setCanvas( m_make_GL_canvas() );
    } else {
        setCanvas(m_make_canvas());
    }
    m_align_scales();

    // Insert grid
    d->grid = new QwtPlotGrid();
    d->grid->attach( this );
}

QuArrayPlot::~QuArrayPlot() {
    delete d;
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
