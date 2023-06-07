#include "qucircularplotdisksector.h"

#include <QPainter>
#include <math.h>
#include <cumacros.h>

class QuCircularPlotDiskSector_P {
public:
    QuCircularPlotDiskSector_P() {
        const QColor c1(Qt::black);
        const QColor c2(45,45,55);
        sections = QStringList{"S1", "S2","S3","S4","S5", "S6",
                               "S7","S8","S9","S10","S11","S12"};
        for(int i = 0; i < sections.size(); i++) {
            colors <<  ((i % 2 == 0) ?  c1 :  c2);
        }
        dirty = true;
        txt_dist = -1.0;
    }

    QStringList sections;
    QVector<QColor> colors;
    float txt_dist;
    bool dirty;
    QRectF rect;
    QFont font;
};

QuCircularPlotDiskSector::QuCircularPlotDiskSector()
    : d(new QuCircularPlotDiskSector_P) {

}

QuCircularPlotDiskSector::~QuCircularPlotDiskSector() {
    delete d;
}

void QuCircularPlotDiskSector::setSections(const QStringList &names) {
    d->sections = names;
    d->dirty = true;
}

void QuCircularPlotDiskSector::setSectionColors(const QVector<QColor> &c) {
    d->colors = c;
}

void QuCircularPlotDiskSector::setTextDist(const float &f) {
    d->txt_dist = f;
}

QVector<QColor> QuCircularPlotDiskSector::sectionColors() const {
    return d->colors;
}

QStringList QuCircularPlotDiskSector::sections() const {
    return d->sections;
}

float QuCircularPlotDiskSector::textDist() const {
    return d->txt_dist;
}

bool QuCircularPlotDiskSector::scales() {
    return true;
}

int QuCircularPlotDiskSector::z() const {
    return -10;
}

void QuCircularPlotDiskSector::draw(QPainter *p, const QuCircularPlotEngine *, double , double outer_radius, const QRectF &rect, QWidget *widget) {
    QPen sp = p->pen();
    QBrush sb = p->brush();
    /*
     * QPainter::drawPie
     *
     * Draws a pie defined by the given rectangle, startAngle and spanAngle.
     * The pie is filled with the current brush().
     * The startAngle and spanAngle must be specified in 1/16th of a degree,
     * i.e. a full circle equals 5760 (16 * 360).
     * Positive values for the angles mean counter-clockwise while negative
     * values mean the clockwise direction. Zero degrees is at the 3 o'clock position.
     */
    float starta = -180 * 16;
    float a = 360.0 / d->colors.size() * 16;
    p->setPen(QPen(Qt::lightGray, 0.0));
    for(int i = 0; i < d->colors.size(); i++) {
        p->setBrush(d->colors[i]);
        p->drawPie(rect, starta -i * a,  -a);
    }
    // restore saved brush
    p->setBrush(sb);
    // restore saved pen
    p->setPen(sp);
    // draw sections
    const QPointF& c = rect.center();
    const float R = qMin(rect.width(), rect.height()) / 2;
    if(d->txt_dist < 0)
        d->txt_dist = (R - outer_radius) / 2.0 + outer_radius ;

    QFontMetrics fm(p->font());
    // if geometry changes, recalculate optimal font size
    if(d->rect != rect || d->dirty) {
        d->font = p->font();
        // maximum horizontal advance: fraction of the distance from center relative to
        // the number of elements in section
        const float hamax = R / (float) (d->sections.size() / 4.0f);
        const float fs = d->font.pointSizeF();
        printf("%s recalculating fonts from %f hamax is %f R is %f divided by %f\n",
               __PRETTY_FUNCTION__, fs, hamax, R, (float) (d->sections.size() / 4.0f));
        int longest_section_idx = -1, ha = 0;
        for(int i = 0; i < d->sections.size(); i++) {
            if(ha < fm.horizontalAdvance(d->sections[i])) {
                ha = fm.horizontalAdvance(d->sections[i]);
                longest_section_idx = i;
            }
        }
        const QString smax = d->sections[longest_section_idx];
        printf(" and longest lable is %s measuring %d\n", qstoc(smax), ha);
        while(fm.height() > hamax /*d->txt_dist / 8.0*/ ||
               fm.horizontalAdvance(smax) >  hamax) {
            d->font.setPointSizeF(d->font.pointSizeF()-0.20);
            fm = QFontMetrics(d->font);
            printf(" to %f (-) ", d->font.pointSizeF());
        }
        while(fs == d->font.pointSizeF() && fm.height() < hamax /*d->txt_dist / 8.0*/
               && fm.horizontalAdvance(smax) < hamax) {
            d->font.setPointSizeF(d->font.pointSizeF() + 0.20);
            fm = QFontMetrics(d->font);
            printf(" to %f (+) ", d->font.pointSizeF());
        }

        d->rect = rect;
        d->dirty = false;
        printf(" and setting font to %f\n", d->font.pointSizeF());
    }
    p->setFont(d->font);
    a = 2 * M_PI / d->sections.size();
    QPointF pt;
    for(int i = 0; i < d->sections.size(); i++) {
        pt.setX(c.x() - fm.horizontalAdvance(d->sections[i])/2.0  - d->txt_dist * cos(a/2.0 + i * a));
        pt.setY(c.y() - d->txt_dist * sin(a/2.0 + i * a));
        p->drawText(pt, d->sections[i]);
    }


}
