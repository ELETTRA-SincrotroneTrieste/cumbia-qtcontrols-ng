#ifndef QUPLOTP_H
#define QUPLOTP_H

#include <qwt_plot_layout.h>
#include <qwt_axis.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_opengl_canvas.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>

class QwtPlotGrid;
class QuCurves;

class QuPlotP {
public:
    QuPlotP(bool ogl, QwtPlot *p) :
        grid(nullptr) {
        if(ogl) {
            p->setCanvas(make_GL_canvas(p) );
        } else {
            p->setCanvas(make_canvas(p));
        }
        align_scales(p);
        int axes[4]{ QwtPlot::xBottom, QwtPlot::xTop, QwtPlot::yLeft, QwtPlot::yRight};
        for(int axisId : axes ) {
            p->axisScaleEngine(axisId)->setAttribute(QwtScaleEngine::Floating);
        }
        // Insert grid
        grid = new QwtPlotGrid();
        grid->attach(p);
    }

    QwtPlotGrid *grid;
    QuCurves *curves;

    QWidget *make_GL_canvas(QwtPlot *p) {
        QwtPlotOpenGLCanvas* canvas = new QwtPlotOpenGLCanvas(p);
        canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
        canvas->setLineWidth( 1 );
        canvas->setPalette( Qt::white );
        return canvas;
    }

    QWidget *make_canvas(QwtPlot *p) {
        QwtPlotCanvas* canvas = new QwtPlotCanvas(p);
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

            // QwtScaleDraw* scaleDraw = plot->axisScaleDraw( axisPos );
            // if ( scaleDraw )
            //     scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
        }

        plot->plotLayout()->setAlignCanvasToScales( true );
    }

};


#endif // QUPLOTP_H
