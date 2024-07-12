#include "arrayplot.h"

// cumbia
#include <cumbiapool.h>
#include <cuserviceprovider.h>
#include <cumacros.h>
#include <quapps.h>
// cumbia

#include <qgridlayout.h>
#include <quarrayplot.h>
#include <quplotdataconnector.h>
#include <quplotoptions.h>
#include <qucurveselector.h>

#include <QCheckBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <qwt_plot_zoomer.h>

Arrayplot::Arrayplot(CumbiaPool *cumbia_pool, QWidget *parent) :
    QWidget(parent) {
    // cumbia
    CuModuleLoader mloader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    cu_pool = cumbia_pool;
    // provide access to the engine in case of runtime swap
    new CuEngineAccessor(this, &cu_pool, &m_ctrl_factory_pool);

    QuArrayPlot *plot1 = new QuArrayPlot(this);
    plot1->setObjectName("plot1");
    QuCurveSelector *p1selector = new QuCurveSelector(plot1);
    connect(p1selector, SIGNAL(selectionChanged()), this, SLOT(curvesSelectionChanged()));
    QuPlotDataConnector *conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, plot1);
    connect(conn, SIGNAL(configured(QwtPlot*,CuData)), this, SLOT(srcConfigured(QwtPlot*,CuData)));


    QGridLayout *lo = new QGridLayout(this);

    QCheckBox *cbhc = new QCheckBox("Highlight selected curve");
    cbhc->setChecked(true);
    QCheckBox *cbmark = new QCheckBox("Marker on selected point");
    cbmark->setChecked(true);
    QCheckBox *cbfollow = new QCheckBox("Auto update marker");
    cbfollow->setChecked(false);
    connect(cbhc, SIGNAL(toggled(bool)), p1selector, SLOT(setHightlightSelectedCurve(bool)));
    connect(cbmark, SIGNAL(toggled(bool)), p1selector, SLOT(setMarkerEnabled(bool)));
    connect(cbfollow, SIGNAL(toggled(bool)), p1selector, SLOT(setMarkerAutoUpdate(bool)));

    QPushButton *pbmovex = new QPushButton("Change curve X axis", this);
    QPushButton *pbmovey = new QPushButton("Change curve Y axis", this);
    connect(pbmovex, SIGNAL(clicked()), this, SLOT(moveCurveX()));
    connect(pbmovey, SIGNAL(clicked()), this, SLOT(moveCurveY()));
    pbmovex->setObjectName("pbmovex"); pbmovey->setObjectName("pbmovey");
    pbmovex->setVisible(false); pbmovey->setVisible(false);


    int row = lo->rowCount();

    QHBoxLayout *hblo = new QHBoxLayout(this);
    hblo->addWidget(cbhc);
    hblo->addWidget(cbmark);
    hblo->addWidget(cbfollow);
    hblo->addWidget(pbmovex);
    hblo->addWidget(pbmovey);
    lo->addLayout(hblo, 0, 0, 1, 10);

    for(int i = 1; i < qApp->arguments().size(); i++)
        conn->addSource(qApp->arguments().at(i).toStdString());
    lo->addWidget(plot1, ++row, 0, 10, 10);

    row = lo->rowCount();
    QuPlotOptions o(plot1);

    foreach(const QString& n, QStringList() << "x" << "y" << "x2" << "y2") {
        int i = 0;
        QCheckBox *cb = new QCheckBox("autoscale " + n, this);
        QDoubleSpinBox *sb = new QDoubleSpinBox(this), *SB = new QDoubleSpinBox(this);
        sb->setMinimum(-10000);
        sb->setMaximum(10000);
        SB->setMinimum(-10000);
        SB->setMaximum(10000);
        sb->setObjectName(plot1->objectName() + "/sb" + n);
        SB->setObjectName(plot1->objectName() + "/SB" + n);

        QPushButton *pba = new QPushButton("Apply", this);
        pba->setObjectName(plot1->objectName() + "/pba" + n);
        lo->addWidget(cb, row, i++, 1, 1);
        lo->addWidget(sb, row, i++, 1, 1);
        lo->addWidget(SB, row, i++, 1, 1);
        lo->addWidget(pba, row++, i++, 1, 1);

        connect(cb, SIGNAL(toggled(bool)), this, SLOT(autoscaleChanged(bool)));
        connect(pba, SIGNAL(clicked()), this, SLOT(scaleChanged()));
        cb->setObjectName(plot1->objectName() + "/cbauto" + n);
    }

    new QwtPlotZoomer(plot1->canvas());

}

Arrayplot::~Arrayplot() {
}

void Arrayplot::autoscaleChanged(bool a) {
    const QString& n = sender()->objectName();
    const QString& plotn = n.section('/', 0, 0);
    const QString& ax = n.section("/cbauto", 1, 1);
    QuArrayPlot *p = findChild<QuArrayPlot *>(plotn);
    pretty_pri("found plot %p sender %s axis %s", p, qstoc(n), qstoc(ax));
    QuPlotOptions o(p);
    if(ax == "x") o.xauto(a);
    else if(ax == "y") o.yauto(a);
    else if(ax == "x2") o.x2auto(a);
    else if(ax == "y2") o.y2auto(a);
    findChild<QPushButton *>(plotn + "/pba" + ax)->setDisabled(a);
    findChild<QDoubleSpinBox *>(plotn + "/sb" + ax)->setDisabled(a);
    findChild<QDoubleSpinBox *>(plotn + "/SB" + ax)->setDisabled(a);
    findChild<QwtPlotZoomer *>()->setZoomBase();
}

void Arrayplot::scaleChanged() {
    const QString& n = sender()->objectName();
    const QString& plotn = n.section('/', 0, 0);
    const QString& ax = n.section("/pba", 1, 1);
    QDoubleSpinBox *sb = findChild<QDoubleSpinBox *>(plotn + "/sb" + ax);
    QDoubleSpinBox *SB = findChild<QDoubleSpinBox *>(plotn + "/SB" + ax);

    QuArrayPlot *p = findChild<QuArrayPlot *>(plotn);
    QuPlotOptions o(p);
    double m = sb->value(), M = SB->value();
    pretty_pri("found plot %p sender %s axis %s setting bounds [%.1f, %.1f]",
               p, qstoc(n), qstoc(ax), m, M);
    if(ax == "x") {
        o.xlo(m); o.xup(M);
    }
    else if(ax == "y") {
        o.ylo(m); o.yup(M);
    }
    else if(ax == "x2") {
        o.x2lo(m); o.x2up(M);
    }
    else if(ax == "y2") {
        o.y2lo(m); o.y2up(M);
    }
    findChild<QwtPlotZoomer *>()->setZoomBase();
}

void Arrayplot::srcConfigured(QwtPlot* p, const CuData& ) {
    const QString& pnam = p->objectName();
    QuPlotOptions o(p);
    foreach(const QString& n, QStringList() << "x" << "y" << "x2" << "y2") {
        QCheckBox *cb = findChild< QCheckBox *>(pnam + "/cbauto" + n);
        QDoubleSpinBox *sb = findChild<QDoubleSpinBox *>(pnam + "/sb" + n),
            *SB = findChild<QDoubleSpinBox *>(pnam + "/SB" + n);
        QPushButton *pba = findChild<QPushButton *>(pnam + "/pba" + n);
        if(n == "x") {
            cb->setChecked(o.xauto());
            if(sb->value() > o.xlo())
                sb->setValue(o.xlo());
            if(SB->value() < o.xup())
                SB->setValue(o.xup());
        }
        if(n == "y"){
            pretty_pri("on checkbox %s auto %s lower %f upper %f",
                       qstoc(cb->objectName()),
                       o.yauto() ? "Y" : "N",
                       o.ylo(), o.yup());
            cb->setChecked(o.yauto());
            if(sb->value() > o.ylo())
                sb->setValue(o.ylo());
            if(SB->value() < o.yup())
                SB->setValue(o.yup());
        }
        if(n == "x2"){
            cb->setChecked(o.x2auto());
            if(sb->value() > o.x2lo())
                sb->setValue(o.x2lo());
            if(SB->value() < o.x2up())
                SB->setValue(o.x2up());
        }
        if(n == "y2"){
            cb->setChecked(o.y2auto());
            if(sb->value() > o.y2lo())
                sb->setValue(o.y2lo());
            if(SB->value() < o.y2up())
                SB->setValue(o.y2up());
        }

        sb->setEnabled(!cb->isChecked());
        SB->setEnabled(!cb->isChecked());
        pba->setEnabled(!cb->isChecked());
    }
    findChild<QwtPlotZoomer *>()->setZoomBase();
}

void Arrayplot::curvesSelectionChanged() {
    const QuCurveSelection& s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData & c, s.data)
        pretty_pri("selected: %s idx %d", qstoc(c.curve->title().text()), c.idx);
    findChild<QPushButton *>("pbmovex")->setVisible(s.size() > 0);
    findChild<QPushButton *>("pbmovey")->setVisible(s.size() > 0);
}

void Arrayplot::moveCurveX() {
    const QuCurveSelection& s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData &c, s.data)
        c.curve->setXAxis(c.curve->xAxis() == QwtPlot::xBottom ? QwtPlot::xTop : QwtPlot::xBottom);
    m_axes_visibility();
}

void Arrayplot::moveCurveY() {
    const QuCurveSelection&  s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData &c, s.data)
        c.curve->setYAxis(c.curve->yAxis() == QwtPlot::yLeft ? QwtPlot::yRight : QwtPlot::yLeft);
    m_axes_visibility();
}

void Arrayplot::m_axes_visibility()
{
    int xbot = 0, xtop = 0, yle = 0, yri = 0;
    foreach(QuCurveSelector *s, findChildren<QuCurveSelector *>()) {
        foreach(QwtPlotCurve *c, s->curves()) {
            if(c->xAxis() == QwtPlot::xBottom) xbot++;
            else if(c->xAxis() == QwtPlot::xTop) xtop++;
            if(c->yAxis() == QwtPlot::yLeft) yle++;
            else if(c->yAxis() == QwtPlot::yRight) yri++;
        }
        s->plot()->setAxisVisible(QwtPlot::xBottom, xbot > 0);
        s->plot()->setAxisVisible(QwtPlot::xTop, xtop > 0);
        s->plot()->setAxisVisible(QwtPlot::yLeft, yle > 0);
        s->plot()->setAxisVisible(QwtPlot::yRight, yri > 0);
    }
}
