#include "quplotdataconnector.h"
#include <cucontext.h>
#include <cucontrolsreader_abs.h>
#include "quarrayplot.h"
#include "quscalarplot.h"

class QuPlotDataConnectorP {
public:
    QuPlotDataConnectorP(QuArrayPlot *p) : ctx(nullptr), type(QuPlotDataConnector::Array) {
        plot.a = p;
    }
    QuPlotDataConnectorP(QuScalarPlot *p) : ctx(nullptr), type(QuPlotDataConnector::Scalar) {
        plot.s = p;
    }
    CuContext *ctx;
    union {
        QuArrayPlot *a;
        QuScalarPlot *s;
    } plot;

    QuPlotDataConnector::DataType type;
};

QuPlotDataConnector::QuPlotDataConnector(CumbiaPool *cp,
                                         const CuControlsFactoryPool &f,
                                         QuArrayPlot* plot) {
    d = new QuPlotDataConnectorP(plot);
    d->ctx = new CuContext(cp, f);
}

QuPlotDataConnector::QuPlotDataConnector(CumbiaPool *cp, const CuControlsFactoryPool &f, QuScalarPlot *plot) {
    d = new QuPlotDataConnectorP(plot);
    d->ctx = new CuContext(cp, f);
}

QuPlotDataConnector::~QuPlotDataConnector() {
    delete d->ctx; // deletes readers
    delete d;
}

void QuPlotDataConnector::setSources(const std::vector<std::string> &l) {
    const std::vector<std::string>& srcs = sources();
    foreach(const std::string& s, l) {
        if(s.length() > 0 && std::find(srcs.begin(), srcs.end(), s) != srcs.end()) {
            CuControlsReaderA* r = d->ctx->add_reader(s, this);
            if(r) r->setSource(s.c_str());
        }
    }
}

void QuPlotDataConnector::setSources(const std::vector<std::string> &l, CuContext *ctx) {
    if(d->ctx)
        delete d->ctx;
    d->ctx = ctx;
    setSources(l);
}

void QuPlotDataConnector::addSource(const std::string &s) {
    CuControlsReaderA* r = d->ctx->add_reader(s, this);
    if(r) r->setSource(s.c_str());
}

void QuPlotDataConnector::unsetSource(const std::string &src) {
    d->ctx->disposeReader(src);
}

void QuPlotDataConnector::unsetSources() {
    d->ctx->disposeReader();
}

std::vector<std::string> QuPlotDataConnector::sources() const {
    std::vector<std::string> l;
    if(!d->ctx)
        return l;
    foreach(CuControlsReaderA *r, d->ctx->readers())
        l.push_back(r->source().toStdString());
    return l;
}

CuContext *QuPlotDataConnector::getContext() const {
    return d->ctx;
}

void QuPlotDataConnector::m_configure(const CuData &data) {
    const std::string& s = data.s(TTT::Src);
    bool err = data.b(TTT::Err);
    if(!err && d->type == Array) {
        d->plot.a->addCurve(s);
    } else if(!err) {
        // trend plot
    }
}

void QuPlotDataConnector::onUpdate(const CuData &data) {
    const std::string& s = data.s(TTT::Src);
    bool err = data.b(TTT::Err);
    if(data.s(TTT::Type) == "property") {
        m_configure(data);
    }
    if(!err && d->type == Array) {
        std::vector<double> y;
        data[TTT::Value].toVector<double>(y);
        d->plot.a->setData(s, y);
    } else if(!err) {
        // trend plot
    }
    else if(err && d->type == Array) {
        d->plot.a->onError(s, data.s(TTT::Message));
        emit newData(d->plot.a, data);
    }
    else if(err && d->type == Scalar) {
        // trend plot
        emit newData(d->plot.s, data);
    }
    if(data.s(TTT::Type) == "property" && d->type == Array)
        emit configured(d->plot.a, data);
    else if(data.s(TTT::Type) == "property" && d->type == Scalar)
        emit configured(d->plot.s, data);
}
