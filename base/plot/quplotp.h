#ifndef QUPLOTP_H
#define QUPLOTP_H

#include <qwt_plot_layout.h>
#include <qwt_axis.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_opengl_canvas.h>
#include <qwt_scale_widget.h>

class QwtPlotGrid;
class QuCurves;

class QuPlotP {
public:
    QuPlotP(bool ogl) :
        opengl(ogl), grid(nullptr) {}

    bool opengl;
    QwtPlotGrid *grid;
    QuCurves *curves;

    QWidget *make_GL_canvas() {
        QwtPlotOpenGLCanvas* canvas = new QwtPlotOpenGLCanvas();
        canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
        canvas->setLineWidth( 1 );
        canvas->setPalette( Qt::white );
        return canvas;
    }

    QWidget *make_canvas() {
        QwtPlotCanvas* canvas = new QwtPlotCanvas();
        canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
        canvas->setLineWidth( 1 );
        canvas->setPalette(Qt::white);
        return canvas;
    }

    //
    //  Set a plain canvas frame and align the scales to it
    //  Taken from Qwt examples/refreshtest
    //
    void align_scales(QwtPlot *plot) {
        // The code below shows how to align the scales to
        // the canvas frame, but is also a good example demonstrating
        // why the spreaded API needs polishing.
        for ( int axisPos = 0; axisPos < QwtAxis::AxisPositions; axisPos++ ) {
            QwtScaleWidget* scaleWidget = plot->axisWidget( axisPos );
            if ( scaleWidget )
                scaleWidget->setMargin( 0 );

            QwtScaleDraw* scaleDraw = plot->axisScaleDraw( axisPos );
            if ( scaleDraw )
                scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
        }

        plot->plotLayout()->setAlignCanvasToScales( true );
    }

};


#endif // QUPLOTP_H
