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
}

Arrayplot::~Arrayplot()
{
}
