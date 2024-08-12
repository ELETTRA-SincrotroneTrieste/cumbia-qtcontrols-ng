#include "plots.h"

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
#include <quscalarplot.h>
#include <qucurves.h>
#include <quplotzoomer.h>
#include <QTimer>

#include <QCheckBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <qwt_plot_zoomer.h>
#include <quxtimescale.h>

Plots::Plots(CumbiaPool *cumbia_pool, QWidget *parent) :
    QWidget(parent) {
    // cumbia
    CuModuleLoader mloader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    cu_pool = cumbia_pool;
    // provide access to the engine in case of runtime swap
    new CuEngineAccessor(this, &cu_pool, &m_ctrl_factory_pool);

    QCommandLineParser parser;
    QCommandLineOption scalar_o(QStringList() << "f" << "format", "s: scalar plots, a: array plots", "a");
    QCommandLineOption datasim_o(QStringList () << "s" << "data-simulate", "if set use simulator not device connection");
    QCommandLineOption bufsiz_o(QStringList() << "b" << "bufsiz", "buffer size (scalar plot only)", "n", "100");
    QCommandLineOption period_o(QStringList() << "p" << "period", "refresh period in ms [default: 1000]", "period", "1000");
    parser.addOption(scalar_o);
    parser.addOption(datasim_o);
    parser.addOption(bufsiz_o);
    parser.addOption(period_o);
    parser.addHelpOption();
    parser.parse(qApp->arguments());
    array = !(parser.value(scalar_o) == "s");
    QwtPlot *plot1 = nullptr;
    QuPlotDataConnector *conn = nullptr;
    QTimer *timer = nullptr;
    if(array) {
        plot1 = new QuArrayPlot(this);
        plot1->setObjectName("plot1");
        conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, static_cast<QuArrayPlot*>(plot1));
    }
    else {
        pretty_pri("creating scalar plot with buffer size of %d",parser.value(bufsiz_o).toInt() );
        plot1 = new QuScalarPlot(this, parser.value(bufsiz_o).toInt());
        plot1->setObjectName("plot1");
        if(parser.isSet(datasim_o)) {
            timer = m_datasim_setup(plot1->objectName(), parser.value(period_o).toInt());
        }
        else {
            conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, static_cast<QuScalarPlot*>(plot1));
            conn->getContext()->setOptions(CuData(TTT::Period, parser.value(period_o).toInt()));
        }
        int x_axes[2] { QwtPlot::xBottom, QwtPlot::xTop};
        for(int i = 0; i < 2; i++)
            new QuXTimeScale(plot1, x_axes[i]);
    }

    QuCurveSelector *p1selector = new QuCurveSelector(plot1);
    connect(p1selector, SIGNAL(selectionChanged()), this, SLOT(curvesSelectionChanged()));

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
    pbmovex->setDisabled(true); pbmovey->setDisabled(true);

    int row = lo->rowCount();

    QHBoxLayout *hblo = new QHBoxLayout(this);
    hblo->addWidget(cbhc);
    hblo->addWidget(cbmark);
    hblo->addWidget(cbfollow);
    hblo->addWidget(pbmovex);
    hblo->addWidget(pbmovey);
    lo->addLayout(hblo, 0, 0, 1, 10);

    foreach(const QString& s, qApp->arguments())
        if(s.count("/") > 2 && !timer)
            conn->addSource(s.toStdString());

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

    for(int i = 0; i < QwtPlot::axisCnt; i++) {
        pretty_pri(" BEFORE SETTING ZOOMER %d: %s", i, plot1->axisAutoScale(i) ? "AUTO" : "MANUAL");
    }
    new QuPlotZoomer(plot1);
    for(int i = 0; i < QwtPlot::axisCnt; i++)
        pretty_pri(" AFTER SETTING ZOOMER %d: %s", i, plot1->axisAutoScale(i) ? "AUTO" : "MANUAL");

    initControls(plot1);

}

Plots::~Plots() {
}

void Plots::autoscaleChanged(bool a) {
    const QString& n = sender()->objectName();
    const QString& plotn = n.section('/', 0, 0);
    const QString& ax = n.section("/cbauto", 1, 1);
    QwtPlot *p = m_plot(plotn);
    pretty_pri("found plot %p sender %s axis %s", p, qstoc(n), qstoc(ax));
    QuPlotOptions o(p);
    if(ax == "x") o.xauto(a);
    else if(ax == "y") o.yauto(a);
    else if(ax == "x2") o.x2auto(a);
    else if(ax == "y2") o.y2auto(a);
    findChild<QPushButton *>(plotn + "/pba" + ax)->setDisabled(a);
    findChild<QDoubleSpinBox *>(plotn + "/sb" + ax)->setDisabled(a);
    findChild<QDoubleSpinBox *>(plotn + "/SB" + ax)->setDisabled(a);
}

void Plots::scaleChanged() {
    const QString& n = sender()->objectName();
    const QString& plotn = n.section('/', 0, 0);
    const QString& ax = n.section("/pba", 1, 1);
    QDoubleSpinBox *sb = findChild<QDoubleSpinBox *>(plotn + "/sb" + ax);
    QDoubleSpinBox *SB = findChild<QDoubleSpinBox *>(plotn + "/SB" + ax);

    QwtPlot *p = m_plot(plotn);
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
}

void Plots::initControls(QwtPlot* p) {
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
}

void Plots::curvesSelectionChanged() {
    const QuCurveSelection& s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData & c, s.data)
        pretty_pri("selected: %s idx %d", qstoc(c.curve->title().text()), c.idx);
    findChild<QPushButton *>("pbmovex")->setEnabled(s.size() > 0);
    findChild<QPushButton *>("pbmovey")->setEnabled(s.size() > 0);
}

void Plots::moveCurveX() {
    const QuCurveSelection& s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData &c, s.data)
        c.curve->setXAxis(c.curve->xAxis() == QwtPlot::xBottom ? QwtPlot::xTop : QwtPlot::xBottom);
    m_axes_visibility();
}

void Plots::moveCurveY() {
    const QuCurveSelection&  s = findChild<QuCurveSelector *>()->selection();
    foreach(const QuCurveSelectionData &c, s.data)
        c.curve->setYAxis(c.curve->yAxis() == QwtPlot::yLeft ? QwtPlot::yRight : QwtPlot::yLeft);
    m_axes_visibility();
}

void Plots::updateScalarPlot() {
    double A = 10; // amplitude
    QTimer *t = qobject_cast<QTimer *>(sender());
    QuScalarPlot *p = findChild<QuScalarPlot *>(t->property("plot").toString());
    int updcnt = t->property("updcnt").toInt();
    double si = A * sin(updcnt / 10.0);
    if(t->property("curve").isNull()) {
        QuCurveSelector sel(p);
        const QString& cnam = QString("curve%1").arg(sel.curves().size());
        p->addCurve(cnam.toStdString());
        t->setProperty("curve", cnam);
    }
    qint64 dt = QDateTime::currentMSecsSinceEpoch();
    findChild<QuScalarPlot *>(t->property("plot").toString())->append(t->property("curve").toString().toStdString(),
                                                                      dt,
                                                                      si);
    // pretty_pri("appending %s: %f\n", qstoc(QDateTime::fromMSecsSinceEpoch(dt).toString("hh:MM:ss.sss")), si);
    t->setProperty("updcnt", updcnt + 1);
}

QTimer* Plots::m_datasim_setup(const QString& plotnam, int period) {
    QTimer *timer = new QTimer(this);
    timer->setObjectName("updateTmr");
    timer->setProperty("updcnt", 0);
    timer->setProperty("plot", plotnam);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScalarPlot()));
    timer->setInterval(period);
    timer->start();
    return timer;
}

QwtPlot *Plots::m_plot(const QString& plotn) const {
    QwtPlot *p =nullptr;
    if(array)  p = findChild<QuArrayPlot *>(plotn);
    else p = findChild<QuScalarPlot *>(plotn);
    return p;
}

void Plots::m_axes_visibility()
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
