#ifndef QUPLOTDATACONNECTOR_H
#define QUPLOTDATACONNECTOR_H

#include <cudatalistener.h>
#include <vector>
#include <string>

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
class QuPlotDataConnector : public CuDataListener
{
public:
    enum DataType { Undefined = -1, Scalar, Array };
    QuPlotDataConnector(CumbiaPool *cp, const CuControlsFactoryPool &f, QuArrayPlot *plot);
    QuPlotDataConnector(CumbiaPool *cp, const CuControlsFactoryPool &f, QuScalarPlot *plot);
    ~QuPlotDataConnector();

    void setSources(const std::vector<std::string> &l);
    void setSources(const std::vector<std::string> &l, CuContext *ctx);

    void addSource(const std::string& s);
    void unsetSource(const std::string& src);
    void unsetSources();

    std::vector<std::string> sources() const;
    CuContext *getContext() const;

private:
    QuPlotDataConnectorP *d;

    void m_configure(const CuData& data);

    // CuDataListener interface
public:
    void onUpdate(const CuData &data);
};

#endif // QUPLOTDATACONNECTOR_H
