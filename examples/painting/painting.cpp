#include "painting.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <qfontmetrics.h>
#include <QResizeEvent>
#include <qgridlayout.h>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include "quzoomer.h"
#include "quzoomevents.h"

ZoomW::ZoomW(QWidget *parent)
    : QWidget(parent)
{
    m_zoom_ev = new QuZoomEvents(this); // event filter
    m_zoomer = new QuZoomer(this);
    connect(m_zoom_ev, SIGNAL(zoomRectChanged(QRectF)), m_zoomer, SLOT(setZoomRect(QRectF)));
    connect(m_zoom_ev, SIGNAL(moveRect(QPointF,QPointF)), m_zoomer, SLOT(moveZoom(QPointF,QPointF)));
    connect(m_zoom_ev, SIGNAL(unzoom()), m_zoomer, SLOT(unzoom()));
            connect(m_zoom_ev, SIGNAL(zoomRectChanging(QPointF,QPointF)), m_zoomer, SLOT(zoomRectChanging(QPointF,QPointF)));
    connect(m_zoomer, SIGNAL(zoomChanged()), this, SLOT(update()));
    m_mode = 1;
    m_rows = m_cols = 30;
}

void ZoomW::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    const QRectF & wr = p.window();
    const QRectF & vr = p.viewport();

    QPen pen(Qt::blue, 4.0);
    p.setPen(pen);
    p.drawRect(wr);

    pen.setWidthF(0.0);
    pen.setColor(Qt::green);
    p.setPen(pen);
    p.drawRect(vr);

    QRect viewport;
    if(m_zoomer->inZoom())
        viewport = m_zoomer->setViewport(&p, event->rect());

    if(viewport != event->rect()) {
        printf("\e[1;35mviewport is %d,%d  %dx%d\e[0m\n", viewport.left(), viewport.top(), viewport.width(),viewport.height());
    }

    pen.setColor(Qt::darkGray);
    p.setPen(pen);
    QFont f = p.font();
    QFontMetrics fm(f);
    int h = fm.horizontalAdvance("xxxxx");
    double i = 2, j = h;
    int cnt = 0;

    const int wid = event->rect().width();
    const int hei = event->rect().height();

    while(m_mode == 0 && j < hei) {
        while(i < wid - fm.horizontalAdvance("xxxxx")) {
            const QString& s = QString::number(cnt++);
            p.drawText(QPointF(i, j), s);
            i += fm.horizontalAdvance("xxxxx");
        }
        i = 2;
        j += h + 2;
    }

    for(int l = 0; l < m_rows && m_mode == 1; l++) {
        for(int k = 0; k < m_cols; k++) {
            const QString& s = QString::number(k + l * 20);
            p.drawText(QPointF(k * h, (l+1) * h), s);
        }
    }

    if(m_mode == 2) {
        pen.setColor(Qt::green);
        p.setPen(pen);
        double r = h;
        while(r < hei && r < wid) {
            p.drawEllipse(QPointF(event->rect().center()), r, r);
            r += h;
            pen.setColor(p.pen().color().darker(105));
            p.setPen(pen);
        }
    }

    if(m_zoomer->rectChanging()) {
        pen.setColor(Qt::darkGray);
        p.setPen(pen);
        p.drawRect(m_zoomer->zoomArea());
    }

}

void ZoomW::resizeEvent(QResizeEvent *re) {
    m_zoomer->sizeChanged(re->size());
}

ZoomW::~ZoomW()
{
}

int ZoomW::nrows() const {
    return m_rows;
}

int ZoomW::ncols() const {
    return m_cols;
}

QSize ZoomW::minimumSizeHint() const {
    return QSize(100,100);
}

void ZoomW::drawModeChanged(int mode) {
    m_mode = mode;
    update();
}

void ZoomW::setNumRows(int r) {
    m_rows = r;
    update();
}

void ZoomW::setNumCols(int c) {
    m_cols = c;
    update();
}


Widget::Widget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *lo = new QHBoxLayout(this);
    ZoomW *zw = new ZoomW(this);
    lo->addWidget(zw, 4);
    // right side: configuration
    QGroupBox *gb = new QGroupBox("configuration", this);
    lo->addWidget(gb, 1);
    QGridLayout *glo = new QGridLayout(gb);
    QLabel *la1 = new QLabel("Draw", gb);
    QComboBox *cb = new QComboBox(gb);
    cb->insertItems(0, QStringList {"Numbers (Adaptive)", "Numbers (fixed)", "Circles"});
    cb->setItemData(0, "Draw a matrix of numbers whose size adapts to the available drawing area", Qt::ToolTipRole);
    cb->setItemData(1, "Draw a matrix of numbers with a fixed number of rows and cols", Qt::ToolTipRole);
    cb->setItemData(2, "Draw Circles that fit the available area", Qt::ToolTipRole);
    connect(cb, SIGNAL(currentIndexChanged(int)), zw, SLOT(drawModeChanged(int)));
    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(drawModeChanged(int)));
    glo->addWidget(la1, 0, 0, 1, 1);
    glo->addWidget(cb, 0, 1, 1, 4);
    resize(800, 600);

}

void Widget::drawModeChanged(int mode) {
    QGroupBox *gb = findChild<QGroupBox *>();
    if(mode == 1) {
        QLabel *lar = new QLabel("rows", gb);
        lar->setObjectName("lrows");
        QSpinBox *sbr = new QSpinBox(gb);
        sbr->setObjectName("sbr");
        sbr->setValue(findChild<ZoomW*>()->nrows());
        QLabel *lac = new QLabel("cols", gb);
        lac->setObjectName("lcols");
        QSpinBox *sbc = new QSpinBox(gb);
        sbc->setObjectName("sbc");
        sbc->setValue(findChild<ZoomW*>()->ncols());
        QGridLayout *glo = gb->findChild<QGridLayout *>();
        const int& row = glo->rowCount();
        glo->addWidget(lar, row, 0, 1, 1);
        glo->addWidget(sbr, row, 1, 1 ,1);
        glo->addWidget(lac, row, 2, 1, 1);
        glo->addWidget(sbc, row, 3, 1, 1);
        connect(sbr, SIGNAL(valueChanged(int)), findChild<ZoomW*>(), SLOT(setNumRows(int)));
        connect(sbc, SIGNAL(valueChanged(int)), findChild<ZoomW*>(), SLOT(setNumCols(int)));

    }
    else {
        delete findChild<QLabel *>("lrows");
        delete findChild<QLabel *>("lcols");
        delete findChild<QSpinBox *>("sbr");
        delete findChild<QSpinBox *>("sbc");
    }
}
