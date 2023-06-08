#include "qucircularplotselectionvalue.h"
#include <QPainter>

#include <qucircularplotengine.h>
#include <qucircularplotcurve.h>

#include <cumacros.h>

QuCircularPlotSelectionValue::QuCircularPlotSelectionValue(QObject *parent) : QObject(parent){
    m_v_changed = false;
    m_i = -1;
    m_format = "%.2f";
}

QString QuCircularPlotSelectionValue::format() const {
    return m_format;
}

void QuCircularPlotSelectionValue::update(QuCircularPlotCurve *c) {
    m_i = c->selected();
    if(m_i > -1) {
        m_v_changed = (m_v != c->y_data()[m_i]);
        if(m_v_changed) {
            m_v = c->y_data()[m_i];
        }
    }
}

void QuCircularPlotSelectionValue::setFormat(const QString &f) {
    m_v_changed = (m_format != f);
    m_format = f;
}

bool QuCircularPlotSelectionValue::scales() {
    return true;
}

int QuCircularPlotSelectionValue::z() const {
    return 10;
}

void QuCircularPlotSelectionValue::draw(QPainter *p, const QuCircularPlotEngine *, double in_r, double , const QRectF &rect, QWidget *) {
    if(m_i >= 0) {
        QFont f = p->font();
        QString n = QString::asprintf(m_format.toStdString().c_str(), m_v);
        QFontMetrics fm(f);
        float len = fm.horizontalAdvance(n);
        const float maxlen = 1.6 * in_r;
        if(m_rect != rect  || m_v_changed) {
            while(fm.height() < maxlen || len < maxlen) {
                f.setPointSize(f.pointSize() + 1);
                fm = QFontMetrics(f);
                len = fm.horizontalAdvance(n);
            }
            while((fm.height() > maxlen || len > maxlen) && f.pointSize() > 1) {
                f.setPointSize(f.pointSize() - 1);
                fm = QFontMetrics(f);
                len = fm.horizontalAdvance(n);
            }
        }
        p->setFont(f);
        const QPointF& c = rect.center();
//        QBrush saveb = p->brush();
//        QColor circlec(Qt::white);
//        circlec.setAlpha(140);
//        p->setBrush(circlec);
//        p->drawEllipse(c.x() - in_r, c.y() - in_r, 2 * in_r, 2 * in_r);
//        p->setBrush(saveb);
        p->drawText(QPointF(c.x() - len / 2.0, c.y() + (float) fm.height() / 4.0), n);

        m_v_changed = false;
    }
}
