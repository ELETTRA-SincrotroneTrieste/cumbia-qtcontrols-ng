#include "qunumericengine.h"
#include <math.h>
#include <QPainter>
#include <QtDebug>
#include <QWidget>
#include <cumacros.h>
#include <QTimer>

#define CHECK_MARK "✓"

QuNumericEngine::QuNumericEngine(const QFont& f)
    : QObject{nullptr}  {
    d.intDig = 3;
    d.decDig = 1;
    d.digits = 4;
    d.data = d.last_applied_data =  0;
    d.min = (long long int) -pow(10.0, d.digits) + 1;
    d.max = (long long int) pow(10.0, d.digits) - 1;
    d.min_d = -pow(10.0, d.intDig) + pow(10.0, -d.decDig);
    d.max_d = pow(10.0, d.intDig) - pow(10.0, -d.decDig);
    d.font_scale = 2.0;
    d.font = f;
    d.font.setPointSizeF(f.pointSizeF() * d.font_scale);
    d.arrow_hei = QFontMetrics(d.font).height() / 4.0;
    d.lm = d.bm = d.um = d.rm = 0.0;
    d.mouse_down = false;
    d.show_apply = true;
    m_update_maxcharw();
}

QuNumericEngine::~QuNumericEngine() {

}

void QuNumericEngine::contextMenuEvent(const QPointF &pos)
{

}

void QuNumericEngine::mousePressEvent(const QPointF &pos) {
    d.mouse_pos = pos;
    d.mouse_down = true;
}

void QuNumericEngine::mouseMoveEvent(const QPointF &pos) {
    d.mouse_pos = pos;
}

void QuNumericEngine::mouseReleaseEvent(const QPointF &pos) {
    d.mouse_pos = pos;
    d.mouse_down = false;
    if(!pos.isNull())
        click(pos);
}

void QuNumericEngine::mouseDoubleClickEvent(const QPointF &pos)
{

}

void QuNumericEngine::wheelEvent(const QPointF &pos, double delta) {
    click(pos, delta > 0);
}

double QuNumericEngine::value() const {
    return d.data * pow(10.0, -d.decDig);
}

double QuNumericEngine::minimum() const {
    return d.min_d;
}

double QuNumericEngine::maximum() const {
    return d.max_d;
}

void QuNumericEngine::paint(QPainter *p, const QRectF &rect, QWidget *widget) {
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
    double t = d.digits + (sign.length() > 0 ? 1 : 0) + (d.decDig > 0 ? dotwr : 0) + (d.show_apply ? 1 : 0);
    digw = w / t;
    dotw = digw * dotwr;

    // draw sign
    if(sign.length() > 0) {
        QuANRect tr(x, 0, digw, h, QuANRect::Sign);
        p->drawText(tr, Qt::AlignHCenter|Qt::AlignVCenter, "-");
        d.digitrects << tr;
        x += tr.width();
    }

    for (int i = 0; i < d.digits; i++)
    {
        if (i == d.intDig)  {
            QuANRect tr(x, 0, dotw, h, QuANRect::Dot);
            p->drawText(tr,  Qt::AlignHCenter|Qt::AlignVCenter, ".");
            d.digitrects << tr;
            x += dotw;
        }
        QuANRect tr(x, 0, digw, h, i), dr(tr);
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
        if(dr.contains(d.mouse_pos)) {
            double x1, x2, y1, y2, h;
            double xoff = dr.width() / 10.0;
            QPen save_pen = p->pen();
            p->setPen(d.mouse_down ? Qt::darkGray : bgcolor.darker(115));
            QColor highlightC(d.mouse_down ? bgcolor.darker(110) : bgcolor.darker(70));
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
        //        p->setPen(Qt::cyan);
        //        p->drawRect(br);

        //
        QPen pe = p->pen();
        pe.setColor(bgcolor);
        pe.setWidthF(fh / 30);
        p->setPen(pe);
        double lw = /*1.1 **/ d.maxcharw, ls = tr.left() + (tr.width() - lw) / 2.0;
        p->drawLine(ls, tr.height()/2.0, ls + lw, tr.height() / 2.0);
        p->setPen(Qt::lightGray);
        p->drawRect(d.digitrects.last());
    }
    if(d.show_apply) {
        p->setPen(Qt::gray);
        const QuANRect apply_r(x, 0, d.digitrects.last().width(), d.digitrects.last().height(), QuANRect::Apply);
        const QBrush b = p->brush();
        const QFont fo = p->font();
        if(apply_r.contains(d.mouse_pos) && d.data != d.last_applied_data) {
            p->setBrush(QBrush(bgcolor.darker(110)));
            p->drawRect(apply_r);
        }
        d.digitrects.push_back(apply_r);
        // draw an apply tick within the font rect
        QFont f(fo);
        f.setPointSizeF(0.75 * fo.pointSizeF());
        p->setFont(f);
        p->setPen(d.data != d.last_applied_data ? QColor(Qt::green) : QColor(Qt::gray));
        p->drawText(apply_r, CHECK_MARK,  Qt::AlignHCenter|Qt::AlignVCenter);
        p->setBrush(b); // restore brush
        p->setFont(fo); // restore font
    }


    p->setPen(Qt::gray);
    p->drawRoundedRect(rect, 1, 1);

}

void QuNumericEngine::m_drawHighlighted() {

}

// returns the index of the digit: right most digit exp 0
int QuNumericEngine::m_map_to_digit_pos(const QPointF &mousePos) const {
    for(int i = 0; i < d.digitrects.size(); i++) {
        if(d.digitrects[i].contains(mousePos) && d.digitrects[i].pos >= 0) {
            return d.digits - d.digitrects[i].pos - 1;
        }
        else if(d.digitrects[i].contains(mousePos))
            return d.digitrects[i].pos; // "+" (pos is negative)
    }
    return QuANRect::Invalid; // "+": pos is < 0
}

bool QuNumericEngine::m_up(const QPointF &mousePos) const {
    if(d.digitrects.size() > 0)
        return mousePos.y() < d.digitrects[0].height() / 2.0;
    return false;
}
void QuNumericEngine::setFont(const QFont &f) {
    d.font = f;
    d.font.setPointSizeF(f.pointSizeF() * d.font_scale);
    d.arrow_hei = QFontMetrics(d.font).height() / 4.0;
    m_update_maxcharw();
    d.minsiz = QSizeF(); // need update minimumSize
}

QSizeF QuNumericEngine::minimumSize()  {
    if(d.minsiz.isValid())
        return d.minsiz;
    qDebug () << __PRETTY_FUNCTION__ << d.font;
    const QString& sign ="-"; // always reserve space for "-" sign to avoid unwanted resizes
    QFontMetrics fm(d.font);
    int fh = fm.height();
    double minh = fh + 2.1 * d.arrow_hei + d.bm + d.um;
    const int nrects = d.show_apply ? d.digits + 1 : d.digits;
    d.minsiz = QSizeF (nrects * d.maxcharw + fm.horizontalAdvance(sign)
               + (d.decDig > 0 ? fm.horizontalAdvance(".") : 0) + d.lm + d.rm, minh);
    qDebug() << __PRETTY_FUNCTION__ << d.font << "pt. siz" << d.font.pointSizeF() << "ascent + descent" << fh
             << "max w" << d.maxcharw << "size width" << d.minsiz.width() ;
    return  d.minsiz;
}

void QuNumericEngine::setValue(double v)
{
    long long temp = (long long) round(v * pow(10.0, d.decDig));
    if ((temp >= d.min) && (temp <= d.max))
    {
        bool valChanged = d.data != temp;
        d.data = temp;
        if (valChanged) {
            emit valueChanged(value());
        }
    }
    else
        perr("%s: value %f out of range [%f, %f] ( %lld [%lld, %lld])",
             __PRETTY_FUNCTION__, v, d.min_d, d.max_d, temp, d.min, d.max);
}

void QuNumericEngine::setMaximum(double m) {
    if (m >= d.min_d && m != d.max_d) {
        d.max_d = m;
        printf("CuApplyNumericEngine::setMaximum to %f. internally from %lld ", m, d.max);
        d.max = (long long) round(m* (long long)pow(10.0, d.decDig));
        printf(" to %lld (%f)\n", d.max, d.max_d);
        emit maximumChanged(d.max_d);
    }
}

void QuNumericEngine::setMinimum(double m) {
    if (m <= d.max_d && m != d.min_d) {
        d.min_d = m;
        printf("CuApplyNumericEngine::setMinimum to %f. internally from %lld ", m, d.min);
        d.min = (long long) round(m* (long long)pow(10.0, d.decDig));
        printf(" to %lld (%f)\n", d.min, d.min_d);
        emit minimumChanged(d.min_d);
    }
}

void QuNumericEngine::setIntDigits(int i)
{
    if (i > 0 && i != d.intDig) {
        d.intDig = i;
        d.digits = d.intDig + d.decDig;
        m_update_bounds();
        d.minsiz = QSizeF(); // need update minimumSize
        // caller needs update()  (paint)
        emit intDigitsChanged(i);
    }
}

void QuNumericEngine::setDecDigits(int dd)
{
    if(dd >= 0  && dd != d.decDig) {
        printf("setDecDigits: dec digits from %d to %d  %lld (%f) ", d.decDig, dd, d.data, value());
        d.data = (long long) (d.data * pow(10.0, dd - d.decDig));
        d.min = (long long) (d.min * pow(10.0, dd - d.decDig));
        d.max = (long long) (d.max * pow(10.0, dd - d.decDig));
        d.decDig = dd;
        d.digits = d.intDig + d.decDig;
        printf("to \e[1;32m%lld (%f)\e[0m\n", d.data, value());
        m_update_bounds();
        // caller needs update()  (paint)
        d.minsiz = QSizeF(); // need update minimumSize
        emit decDigitsChanged(dd);
    }
}

void QuNumericEngine::click(const QPointF& pos) {
    bool up = m_up(pos);
    click(pos, up);
}

void QuNumericEngine::click(const QPointF &pos, bool up) {
    printf("CuApplyNumericEngine::click \e[1;35m %lld (%f) \e[1;34m ... ", d.data, value());
    if(!pos.isNull()) {
        int dig = m_map_to_digit_pos(pos);
        printf(" Dig is %d ", dig);
        if (dig > -1 && up && (d.data + pow(10.0, dig)) <= d.max) {
            d.data += (long long) pow(10.0, dig);
            emit valueChanged(value());
        }
        else if(dig > -1 && !up && d.data - pow(10.0, dig) >= d.min) {
            d.data -= (long long) pow(10.0, dig);
            emit valueChanged(value());
        }
        else if(m_rect_type(dig) == QuANRect::Apply) {
            d.last_applied_data = d.data;
            emit applyClicked(value());
        }
    }
    printf("\e[1;32m to %lld (%f)\e[0m\n", d.data, value());
}

void QuNumericEngine::setShowApply(bool show) {
    d.show_apply = show;
    m_update_maxcharw();
    d.minsiz = QSizeF(); // need update minimumSize
}

void QuNumericEngine::m_update_bounds()  {
    bool c;
    long long int m = (long long int) -pow(10.0, d.digits) + 1;
    long long int M =  (long long int) pow(10.0, d.digits) - 1;
    printf("\e[0;33mm_update_bounds old m %lld --> %lld \e[0;32m old M %lld --> %lld\e[0m", d.min, m, d.max, M);
    if(m > d.min) {
        d.min = m;
        d.min_d = -pow(10.0, d.intDig) + pow(10.0, -d.decDig);
        emit minimumChanged(d.min_d);
    }
    if(M < d.max) {
        d.max = M;
        d.max_d = pow(10.0, d.intDig) - pow(10.0, -d.decDig);
        emit maximumChanged(d.max_d);
    }
    printf("\e[0;36m %lld < %lld < %lld \e[1;35m %f < %f < %f\e[0m\n", d.min, d.data, d.max, d.min_d, value() , d.max_d);
    c = d.data < d.min || d.data > d.max;
    if(d.data < d.min)
        d.data = d.min;
    if(d.data > d.max)
        d.data = d.max;
    if(c) {
        emit valueChanged(value());
        printf("\e[1;31mdata changed to %lld ( %f ) due to bounds changed \e[0m\n", d.data, value());
    }
    printf("with %d int digs and %d dec digs: \e[1;35mm_update_bounds %.3f - %.3f\e[0m\n", d.intDig, d.decDig, d.min_d, d.max_d);
}

double QuNumericEngine::m_to_double(long long int lli) const {
    return lli * pow(10.0, -d.decDig);
}

void QuNumericEngine::m_update_maxcharw() {
    QFontMetrics fm(d.font);
    d.maxcharw = 0;
    double w;
    for(int i = 0; i < 10; i++) {
        w = fm.horizontalAdvance(QString("%1").arg(i));
        if(d.maxcharw < w)
            d.maxcharw = w;
    }
    if(d.show_apply && d.maxcharw < fm.horizontalAdvance(CHECK_MARK))
        d.maxcharw = fm.horizontalAdvance(CHECK_MARK);
}

QuANRect::Type QuNumericEngine::m_rect_type(int idx) const {
    if(idx >= 0)
        return QuANRect::Digit;
    return static_cast<QuANRect::Type> (idx);
}

