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
#include <math.h>

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
    const QTransform ct = p.combinedTransform();
    if(m_zoomer->inZoom()) {
        p.setTransform(m_zoomer->calculateTransform(event->rect()) * ct);
    }
    QPen pen(Qt::darkGray, 0.0);
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

    if(m_mode == 3 || m_mode == 4) {
        if(m_img.isNull())
            m_img = QImage(":seoyeji.png");
        if(m_mode == 3) {
            p.drawImage(event->rect(), m_img);
            int w = event->rect().width();
            int h = event->rect().height();
            float iw = m_img.width();
            float ih = m_img.height();
            float wr = iw / w;  // width ratio
            float hr = ih / h; // height ratio
            float d = fabs(wr - hr);
            printf("event rect w %d height %d - wr %f hr %f d %f\n", w, h, wr, hr, d);
            pen.setColor(d > 0.005 ? Qt::red : Qt::green);
            pen.setWidthF(6);
            p.setPen(pen);
            wr < hr ? w = iw /hr : h = ih / wr;
            QRect ire(event->rect().topLeft(), QSize(w, h));
            p.drawRect(ire);
        }
        else
            p.drawImage(m_img.rect(), m_img);
    }

    if(m_zoomer->rectChanging()) {
        pen.setWidthF(0.0);
        pen.setColor(Qt::darkGray);
        p.setPen(pen);
        p.drawRect(m_zoomer->zoomArea());
    }

    if(m_zoomer->inZoom()) {
        p.setTransform(ct);  // restore transform
        QPen pe(Qt::blue, 2.0);
        pe.setStyle(Qt::DashDotLine);
        p.setPen(pe);
        QRect zr(event->rect());
        zr.setX(zr.x() + pe.widthF() / 2.0);
        zr.setY(zr.y() + pe.widthF() / 2.0);
        zr.setRight(zr.right() - pe.widthF());
        zr.setBottom(zr.bottom() - pe.widthF());
        p.drawRect(zr);
    }
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
    cb->insertItems(0, QStringList {"Numbers (Adaptive)", "Numbers (fixed)", "Circles", "Image (adaptive)", "Image (fixed)"});
    cb->setItemData(0, "Draw a matrix of numbers whose size adapts to the available drawing area", Qt::ToolTipRole);
    cb->setItemData(1, "Draw a matrix of numbers with a fixed number of rows and cols", Qt::ToolTipRole);
    cb->setItemData(2, "Draw Circles that fit the available area", Qt::ToolTipRole);
    cb->setItemData(3, "Draw an image that fits to the available area", Qt::ToolTipRole);
    cb->setItemData(4, "Draw a fixed size image", Qt::ToolTipRole);

    QLabel *limgi = new QLabel("Resize and follow the frame until it turns green to scale the image properly");
    limgi->setObjectName("limage");
    limgi->setWordWrap(true);
    limgi->setVisible(false);

    connect(cb, SIGNAL(currentIndexChanged(int)), zw, SLOT(drawModeChanged(int)));
    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(drawModeChanged(int)));
    glo->addWidget(la1, 0, 0, 1, 1);
    glo->addWidget(cb, 0, 1, 1, 4);
    glo->addWidget(limgi, 1, 0, 1, 5);
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
    findChild<QLabel *>("limage")->setVisible(mode == 3);
}
