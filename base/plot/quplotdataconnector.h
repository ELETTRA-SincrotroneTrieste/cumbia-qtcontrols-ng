#ifndef QUPLOTDATACONNECTOR_H
#define QUPLOTDATACONNECTOR_H

#include "qwt_plot.h"
#include <cudatalistener.h>
#include <vector>
#include <string>
#include <QObject>

class CuContext;
class CumbiaPool;
class CuControlsFactoryPool;
class QuArrayPlot;
class QuScalarPlot;

class QuPlotDataConnectorP;

/*!
 * \brief connect data sources to a plot
 *
 * Inspired by cumbia-qtcontrols QuPlotCommon
 */
class QuPlotDataConnector : public QObject, CuDataListener
{
    Q_OBJECT
public:
    enum DataType { Undefined = -1, Scalar, Array };
    QuPlotDataConnector(CumbiaPool *cp, const CuControlsFactoryPool &f, QuArrayPlot *plot);
    QuPlotDataConnector(CumbiaPool *cp, const CuControlsFactoryPool &f, QuScalarPlot *plot);
    ~QuPlotDataConnector();

    std::vector<std::string> sources() const;
    CuContext *getContext() const;

public slots:
    void setSources(const std::vector<std::string> &l);
    void setSources(const std::vector<std::string> &l, CuContext *ctx);
    void addSource(const std::string& s);
    void unsetSource(const std::string& src);
    void unsetSources();

signals:
    /*! sent after new data has been processed by onUpdate
     */
    void newData(QwtPlot* plot, const CuData& );

    /*! shorthand when data contains the TTT::Type set to "property"
     *  newData is however emitted contextually
     */
    void configured(QwtPlot* plot, const CuData& );

private:
    QuPlotDataConnectorP *d;

    void m_configure(const CuData& data);

    // CuDataListener interface
public:
    void onUpdate(const CuData &data);
};

#endif // QUPLOTDATACONNECTOR_H
