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

#include <QCheckBox>
#include <QLabel>

Arrayplot::Arrayplot(CumbiaPool *cumbia_pool, QWidget *parent) :
    QWidget(parent) {
    // cumbia
    CuModuleLoader mloader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    cu_pool = cumbia_pool;
    // provide access to the engine in case of runtime swap
    new CuEngineAccessor(this, &cu_pool, &m_ctrl_factory_pool);

    QGridLayout *lo = new QGridLayout(this);
    QuArrayPlot *plot = new QuArrayPlot(this);
    QuPlotDataConnector *conn = new QuPlotDataConnector(cu_pool, m_ctrl_factory_pool, plot);
    for(int i = 1; i < qApp->arguments().size(); i++)
        conn->addSource(qApp->arguments().at(i).toStdString());
    lo->addWidget(plot, 0, 0, 10, 10);
    // mloader.modules() to get the list of loaded modules
    // cumbia
    QLabel *lauto = new QLabel("autoscale", this);
    int i = 2, row = lo->rowCount();
    QuPlotOptions o(plot);
    foreach(const QString& n, QStringList() << "x" << "y" << "x2" << "y2") {
        QCheckBox *cb = new QCheckBox(n, this);
        if(n == "x") cb->setChecked(o.xauto());
        if(n == "y") cb->setChecked(o.yauto());
        if(n == "x2") cb->setChecked(o.x2auto());
        if(n == "y2") cb->setChecked(o.y2auto());
        lo->addWidget(cb, row, i++, 1, 1);
        connect(cb, SIGNAL(toggled(bool)), this, SLOT(autoscaleChanged(bool)));
        cb->setObjectName(n);
    }
    lo->addWidget(lauto, row, 0, 1, 2);
}

Arrayplot::~Arrayplot()
{
}

void Arrayplot::autoscaleChanged(bool a) {
    const QString& n = sender()->objectName();
    foreach(QuArrayPlot *p, findChildren<QuArrayPlot *>()) {
        QuPlotOptions o(p);
        if(n == "x") o.xauto(a);
        else if(n == "y") o.yauto(a);
        else if(n == "x2") o.x2auto(a);
        else if(n == "y2") o.y2auto(a);
    }
}
