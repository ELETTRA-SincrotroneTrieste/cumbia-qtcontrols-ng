#include "qucurveselector.h"
#include "qpen.h"
#include "qwt_text.h"
#include <qwt_plot.h>
#include <QMouseEvent>
#include <cumacros.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

class QuCurveSelectorP {
public:
    QuCurveSelectorP(bool hp, bool hc) :
        hlight_c(hc), hlight_p(hp),
        marker_follows(true), moved(false),
        marker(nullptr) {}
    bool hlight_c, hlight_p, marker_follows, moved;
    QuCurveSelection selection;
    QwtPlotMarker *marker;
};

QuCurveSelector::QuCurveSelector(QwtPlot *plot, bool highlight_point, bool highlight_curve)
    : QObject{plot}, d(new QuCurveSelectorP(highlight_point, highlight_curve)) {
    plot->canvas()->installEventFilter(this);
}

QuCurveSelector::~QuCurveSelector() {
    delete d;
}

bool QuCurveSelector::eventFilter(QObject *o, QEvent *e) {
    if(plot() == nullptr || o != plot()->canvas())
        return false;
    switch(e->type()) {
    case QEvent::MouseButtonPress:
        d->moved = false;
        break;
    case QEvent::MouseButtonRelease: {
        if(d->moved)
            break;

        m_highlight_selected_c(false);
        m_set_marker(false);

        QuCurveSelection olds = d->selection;
        const QMouseEvent *me = static_cast<QMouseEvent *>(e);
        me->button() != Qt::LeftButton ? deselect() : select(me->pos());

        bool replot = m_highlight_selected_c(d->hlight_c);
        replot |= m_set_marker(d->hlight_p);
        if(replot)
            plot()->replot();

        if(!(olds == d->selection))
            emit selectionChanged();
        break;
    }
    case QEvent::MouseMove: {
        d->moved = true;
        break;
    }
    default:
        break;
    }
    return QObject::eventFilter(o, e);
}

void QuCurveSelector::setHightlightSelectedCurve(bool hl) {
    d->hlight_c = hl;
    m_highlight_selected_c(hl);
    plot()->replot();
}

void QuCurveSelector::setMarkerEnabled(bool m) {
    d->hlight_p = m;
    m_set_marker(m);
    plot()->replot();
}

void QuCurveSelector::setMarkerAutoUpdate(bool f) {
    d->marker_follows = f;
    if(!f)
        disconnect(plot(), SIGNAL(dataUpdated(QwtPlotCurve*)), this, SLOT(dataUpdated(QwtPlotCurve*)));
    else
        connect(plot(), SIGNAL(dataUpdated(QwtPlotCurve*)), this, SLOT(dataUpdated(QwtPlotCurve*)));
}

void QuCurveSelector::select(const QPoint &pos) {
    d->selection.clear();
    double dist = 10e10;
    foreach ( QwtPlotCurve *c, curves())  {
        if ( c->isVisible() ) {
            double di;
            int idx = c->closestPoint( pos, &di );
            if(di == dist) {
                d->selection.add(c, idx);
            }
            else if( di < dist ) {
                d->selection.clear();
                d->selection.add(c, idx);
                d->selection.pos = pos;
                dist = di;
            }
        }
    }
}

void QuCurveSelector::deselect() {
    int oldss = d->selection.size();
    m_highlight_selected_c(false);
    d->selection.clear();
    if(oldss > 0)
        plot()->replot();
}

QuCurveSelection QuCurveSelector::selection() const {
    return d->selection;
}

QwtPlot *QuCurveSelector::plot() const {
    return qobject_cast<QwtPlot *>(parent());
}

QList<QwtPlotCurve *> QuCurveSelector::curves() const
{
    QList<QwtPlotCurve *> l;
    const QwtPlotItemList& itmList = plot()->itemList();
    for ( QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it )
        if ( ( *it )->rtti() == QwtPlotItem::Rtti_PlotCurve )
            l.push_back(static_cast<QwtPlotCurve *>(*it));
    return l;
}

bool QuCurveSelector::m_highlight_selected_c(bool h) {
    bool replot = false;
    const QList<QwtPlotCurve *> l = curves();
    for(QwtPlotCurve *c : l) {
        int idx = d->selection.indexOf(c);
        replot |= (idx > -1);
        if(idx > -1) {
            float w = d->selection.data[idx].penw;
            QPen p = c->pen();
            p.setWidthF(h ? (w > 0 ? w * 2.8 : 2.8) : w);
            c->setPen(p);
        } else {

        }
    }
    return replot;
}

bool QuCurveSelector::m_set_marker(bool m) {
    bool replot = false;
    const QList<QwtPlotCurve *> l = curves();
    for(QwtPlotCurve *c : l) {
        int idx = d->selection.indexOf(c);
        replot |= (idx > -1);
        if(idx > -1) {
            if(!d->marker && m) {
                d->marker = new QwtPlotMarker();
                d->marker->attach(plot());
                d->marker->setSymbol(new QwtSymbol(QwtSymbol::Cross,
                                                   QBrush(QColor(Qt::green)),
                                                   QPen(QColor(Qt::green)),
                                                   QSize(20,20)));
                d->marker->setLinePen(c->pen().color(), 0.0);
                d->marker->setLineStyle(QwtPlotMarker::Cross);
                const QPointF& s = c->sample(d->selection.data[idx].idx);
                d->marker->setValue(s);
                d->marker->setLabel(QString("%1\n%2").arg(s.x()).arg(s.y()));
            }
            else if(d->marker && !m) {
                delete d->marker;
                d->marker = nullptr;
            }
        } else {

        }
    }
    return replot;
}

void QuCurveSelector::dataUpdated(QwtPlotCurve *c) {
    if(d->selection.size() > 0 && d->marker) {
        const int i = d->selection.indexOf(c);
        if(i > -1) {
            size_t idx = d->selection.data[i].idx;
            if(idx < c->dataSize()) {
                d->marker->setValue(c->sample(idx));
                plot()->replot();
            }
        }

    }
}
