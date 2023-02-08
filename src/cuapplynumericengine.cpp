#include "cuapplynumericengine.h"
#include <math.h>
#include <QPainter>
#include <QtDebug>
#include <QWidget>

CuApplyNumericEngine::CuApplyNumericEngine(const QFont& f)
    : QObject{nullptr}  {
    d.intDig = 3;
    d.decDig = 1;
    d.digits = 4;
    d.data = 0;
    d.minVal = (long long int) -pow(10.0, d.digits) + 1;
    d.maxVal = (long long int) pow(10.0, d.digits) - 1;
    d.d_minAsDouble = (double) roundl(d.minVal);
    d.d_maxAsDouble = (double) roundl(d.maxVal);
    d.font_scale = 2.0;
    d.font = f;
    d.font.setPointSizeF(f.pointSizeF() * d.font_scale);
    d.arrow_hei = QFontMetrics(d.font).height() / 4.0;
    d.lm = d.bm = d.um = d.rm = 0.0;
    d.mouse_down = false;
}

CuApplyNumericEngine::~CuApplyNumericEngine() {

}

void CuApplyNumericEngine::contextMenuEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::mousePressEvent(const QPointF &pos) {
    d.mouse_pos = pos;
    d.mouse_down = true;
}

void CuApplyNumericEngine::mouseMoveEvent(const QPointF &pos) {
    d.mouse_pos = pos;
}

void CuApplyNumericEngine::mouseReleaseEvent(const QPointF &pos) {
    d.mouse_pos = pos;
    d.mouse_down = false;
}

void CuApplyNumericEngine::mouseDoubleClickEvent(const QPointF &pos)
{

}

void CuApplyNumericEngine::wheelEvent(const QPointF &pos, double delta)
{

}

void CuApplyNumericEngine::paint(QPainter *p, const QRectF &rect, QWidget *widget) {
    QColor bgcolor = widget->palette().color(QPalette::Window);
    QFont f = p->font();
    f.setPointSizeF(f.pointSizeF() * d.font_scale);
    p->setFont(f);
    p->setRenderHint(QPainter::Antialiasing, true);
    QFontMetrics fm(f);
    double fh =  fm.ascent() + fm.descent();
    d.digitrects.clear();

    double x = 0; // current digit position during loop
    long long temp = d.data;
    int num = 0;
    const QString& sign = (d.data < 0) ? "-" : "";
    // calculate rects for digits
    double digw, dotw; // widths for sign, digits and dot
    const double& w = rect.width(); // available width
    const double& h = rect.height();
    // estimate the proportion between the width needed for the dot and a normal digit
    // give the sign the same width as a digit
    double dotwr = fm.horizontalAdvance(".") / (double) fm.horizontalAdvance('8');
    double t = d.digits + (sign < 0 ? 1 : 0) + (d.decDig > 0 ? dotwr : 0);
    digw = w / t;
    dotw = digw * dotwr;

    // draw sign
    if(sign.length() > 0) {
        QRectF tr(x, 0, digw, h);
        p->drawText(tr, Qt::AlignHCenter|Qt::AlignVCenter, "-");
        d.digitrects << tr;
        x += tr.width();
    }

    for (int i = 0; i < d.digits; i++)
    {
        if (i == d.intDig)  {
            QRectF tr(x, 0, dotw, h);
            p->drawText(tr,  Qt::AlignHCenter|Qt::AlignVCenter, ".");
            d.digitrects << tr;
            x += dotw;
        }
        QRectF tr(x, 0, digw, h), dr(tr);
        dr.setLeft(dr.left() + 2);
        dr.setTop(dr.top() + 2);
        dr.setBottom(dr.bottom() - 1);
        dr.setRight(dr.right() - 1);
        QRectF r(tr);
        p->setPen(Qt::black);
        num = (long long) (temp / (long long) pow(10.0, d.digits-i-1));
        temp -= num * (long) pow(10.0, d.digits-i-1);
        const QString& txt = QString("%1").arg(abs(num)); // draw after highlight
        tr.setRect(x, 0, digw, h);
        d.digitrects << tr;
        x += tr.width();
        qDebug() << __PRETTY_FUNCTION__ << dr << "contains mouse pos ? " << dr.contains(d.mouse_pos);

        if(dr.contains(d.mouse_pos)) {
            double x1, x2, y1, y2, h;
            double xoff = dr.width() / 10.0;
            QPen save_pen = p->pen();
            p->setPen(d.mouse_down ? Qt::darkGray : bgcolor.darker(115));
            QColor highlightC(d.mouse_down ? bgcolor.darker(110) : bgcolor.darker(70));
//            highlightC.setAlpha(120);
            QBrush savebrush = p->brush();
            p->setBrush(highlightC);
            if(d.mouse_pos.y() > dr.height() / 2.0) {
                p->drawRoundedRect(r.x(), r.y() + r.height() / 2.0, r.width(), r.height() / 2.0, 1.0, 1.0);
                // arrow down
                p->setPen(bgcolor.darker(180));
                x1 = dr.left() + xoff;
                x2 = x1 + (dr.width() - 2 * xoff) / 2.0;
                y1 = dr.bottom() - dr.height() / 2.0 + fh / 2.0 ;
                d.arrow_hei < (dr.height() - fh)/2.0 ? h = d.arrow_hei : h = (dr.height() - fh)/2.0;
                y2 = y1 + h;
                p->drawLine(x1, y1, x2, y2);
                x1 = x2;
                y1 = y2;
                x2 = dr.right() - xoff;
                y2 = y1 - h;
                p->drawLine(x1, y1, x2, y2);
            }
            else {
                p->drawRoundedRect(r.x(), r.y(), r.width(), r.height() / 2.0, 1.0, 1.0);
                // arrow up
                p->setPen(bgcolor.darker(180));
                x1 = dr.left() + xoff;
                x2 = x1 + (dr.width() - 2 * xoff) / 2.0;
                d.arrow_hei < (dr.height() - fh)/2.0 ? h = d.arrow_hei : h = (dr.height() - fh)/2.0;
                y1 = dr.top() + dr.height() / 2.0 - fh / 2.0;
                y2 = y1 - h;
                p->drawLine(x1, y1, x2, y2);
                x1 = x2;
                y1 = y2;
                x2 = dr.right() - xoff;
                y2 = y1 + h;
                p->drawLine(x1, y1, x2, y2);
            }
            p->setPen(save_pen);
            p->setBrush(savebrush);
        }


        //        QRectF br; // text bounding rect
        p->drawText(tr,  Qt::AlignHCenter|Qt::AlignVCenter, txt/*, &br*/);
        d.digitrects << tr;
        //        p->setPen(Qt::cyan);
        //        p->drawRect(br);

        //
        QPen pe = p->pen();
        pe.setColor(bgcolor);
        pe.setWidthF(fh / 16);
        p->setPen(pe);
        double lw = 0.9 * tr.width(), ls = x + (tr.width() - lw) / 2.0;
        p->drawLine(ls, tr.height()/2.0, ls + lw, tr.height() / 2.0);
        p->setPen(Qt::lightGray);
        p->drawRect(d.digitrects.last());

        //        QPushButton *temp = new QPushButton(this);
        //        temp->setObjectName(QString("layoutmember") + QString().setNum(i));
        //        temp->installEventFilter(lCWME);
        //        bup->addButton(temp);

        //        l = new QLabel(QString().setNum(i), this);
        //        l->setObjectName(QString("layoutmember") + QString().setNum(i));
        //        labels.push_back(l);

        //        QPushButton *temp2 = new QPushButton(this);
        //        temp2->setObjectName(QString("layoutmember") + QString().setNum(i));
        //        temp2->installEventFilter(lCWME);
        //        bdown->addButton(temp2);

        //        formatDigit(temp, l, temp2);
        //        if (i < intDig)
        //        {
        //            box->addWidget(temp, 0, i + 1);
        //            box->addWidget(l, 1, i + 1);
        //            box->addWidget(temp2, 2, i + 1);
        //        }
        //        else if (i >= intDig)
        //        {
        //            box->addWidget(temp, 0, i + 2);
        //            box->addWidget(l, 1, i + 2);
        //            box->addWidget(temp2, 2, i + 2);
        //        }

        //        if (i == 0)
        //        {
        //            /* messo qui per evitare casini col designer */
        //            signLabel = new QLabel("+", this);
        //            signLabel->setAlignment(Qt::AlignCenter);
        //            signLabel->setObjectName("layoutmember+");
        //            box->addWidget(signLabel, 1, 0);
        //        }
    }


    p->setPen(Qt::gray);
    p->drawRoundedRect(rect, 1, 1);

}

void CuApplyNumericEngine::m_drawHighlighted() {

}

void CuApplyNumericEngine::setFont(const QFont &f) {
    d.font = f;
    d.font.setPointSizeF(f.pointSizeF() * d.font_scale);
    d.arrow_hei = QFontMetrics(d.font).height() / 4.0;
}

QSizeF CuApplyNumericEngine::minimumSize() const {
    qDebug () << __PRETTY_FUNCTION__ << d.font;
    const QString& sign = (d.data < 0) ? "-" : "";
    QFontMetrics fm(d.font);
    int fh = fm.height();
    double minh = fh + 2.1 * d.arrow_hei + d.bm + d.um;
    double maxw = 0, w;
    for(int i = 0; i < 10; i++) {
        w = fm.horizontalAdvance(QString("%1").arg(i));
        if(maxw < w)
            maxw = w;
    }
    QSizeF siz(d.digits * maxw + fm.horizontalAdvance(sign)
               + (d.decDig > 0 ? fm.horizontalAdvance(".") : 0) + d.lm + d.rm, minh);
    qDebug() << __PRETTY_FUNCTION__ << d.font << "pt. siz" << d.font.pointSizeF() << "ascent + descent" << fh
             << "max w" << maxw << "size width" << siz.width() ;
    return  siz;
}

