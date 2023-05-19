#include "painting.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <qfontmetrics.h>
#include "quzoomer.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_zoomer = new QuZoomer(this);
}

void Widget::paintEvent(QPaintEvent *event) {
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

    printf("\e[1;32m zoomer in zoom?? %s\e[0m\n", m_zoomer->inZoom() ? "YES" : "NO");
    if(m_zoomer->inZoom()) {
        pen.setColor(Qt::magenta);
        p.setPen(pen);
        p.drawRect(m_zoomer->zoomRect());
        m_zoomer->zoom(&p, event->rect(), this);
    }


    pen.setColor(Qt::darkGray);
    p.setPen(pen);
    QFont f = p.font();
    QFontMetrics fm(f);
    int h = fm.horizontalAdvance("xxxxx");
    double i = 2, j = h;
    int cnt = 0;
    while(j < height()) {
        while(i < width() - fm.horizontalAdvance("xxxxx")) {
            const QString& s = QString::number(cnt++);
            p.drawText(QPointF(i, j), s);
            i += fm.horizontalAdvance("xxxxx");
        }
        i = 2;
        j += h + 2;
    }
    if(m_zoomer->p1 != m_zoomer->p2)
        m_zoomer->drawZoomRect(&p, event->rect(), this);
}

Widget::~Widget()
{
}

