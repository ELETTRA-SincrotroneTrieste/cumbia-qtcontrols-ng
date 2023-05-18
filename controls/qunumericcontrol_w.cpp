#include "qunumericcontrol_w.h"
#include <cumacros.h>
#include <cudata.h>
#include <cumbia.h>
#include <cuserviceprovider.h>

#include <number_delegate.h>
#include "cucontrolswriter_abs.h"
#include "cucontrolsfactories_i.h"
#include "cucontrolsutils.h"
#include "cumbiapool.h"
#include "cucontext.h"
#include "culinkstats.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <culog.h>

/// @private
class QuNumericControlW_P
{
public:
    CuContext *context;
    bool auto_configure;
    bool write_ok;
    CuLog *log;
    CuControlsUtils u;
};

/** \brief Constructor with the parent widget, an *engine specific* Cumbia implementation and a CuControlsWriterFactoryI interface.
 *
 *  Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuNumericControlW::QuNumericControlW(QWidget *parent, Cumbia *cumbia, const CuControlsWriterFactoryI &w_fac)
    : QuNumericW(parent)
{
    m_init();
    d->context = new CuContext(cumbia, w_fac);
}

/** \brief Constructor with the parent widget, *CumbiaPool*  and *CuControlsFactoryPool*
 *
 *   Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuNumericControlW::QuNumericControlW(QWidget *parent, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool)
    : QuNumericW(parent)
{
    m_init();
    d->context = new CuContext(cumbia_pool, fpool);
}

/*! \brief class destructor
 *
 * Deletes the context used to link the widget to the writer.
 */
QuNumericControlW::~QuNumericControlW()
{
    delete d->context;
    delete d;
}

/*! \brief returns the target name.
 *
 * @return the target name, or an empty string if setTarget hasn't been called yet.
 *
 * Refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QString QuNumericControlW::target() const
{
    CuControlsWriterA *w = d->context->getWriter();
    if(w != NULL)
        return w->target();
    return "";
}

/** \brief Set the name of the target that will be executed.
 *
 * Refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
void QuNumericControlW::setTarget(const QString &target)
{
    CuControlsWriterA* w = d->context->replace_writer(target.toStdString(), this);
    if(w) w->setTarget(target);
}

/*! \brief executes the target specified with setTarget, writing the value set on the widget.
 *
 * The value set on the widget is written on the target when the *apply* button
 * is clicked.
 * This virtual method can be reimplemented on a subclass.
 */
void QuNumericControlW::execute(double val) {
    CuVariant args(val);
    CuControlsWriterA *w = d->context->getWriter();
    if(w) {
        w->setArgs(args);
        w->execute();
    }
}

void QuNumericControlW::onAnimationValueChanged(const QVariant &) {

}

/// @private
void QuNumericControlW::m_init()
{
    d = new QuNumericControlW_P;
    connect(this, SIGNAL(clicked(double)), this, SLOT(execute(double)));
    d->auto_configure = true;
    d->write_ok = false;
}

/*! \brief configures the widget as soon as it is connected and records write errors.
 *
 * @param da CuData containing configuration options upon link creation or
 *        write operation results.
 */
void QuNumericControlW::onUpdate(const CuData &da)
{
    bool is_config = da["type"].toString() == std::string("property");
    if(!da["is_result"].toBool() && !is_config)
        return;

    d->write_ok = !da["err"].toBool();
    // update link statistics
    d->context->getLinkStats()->addOperation();
    const QString& msg = d->u.msg(da);
    if(!d->write_ok)
    {
        perr("QuNumericControlW [%s]: error %s target: \"%s\" format %s (writable: %d)", qstoc(objectName()),
             da["src"].toString().c_str(), msg.toStdString().c_str(),
                da["dfs"].toString().c_str(), da["writable"].toInt());

        Cumbia* cumbia = d->context->cumbia();
        if(!cumbia) /* pick from the CumbiaPool */
            cumbia = d->context->cumbiaPool()->getBySrc(da["src"].toString());
        CuLog *log;
        if(cumbia && (log = static_cast<CuLog *>(cumbia->getServiceProvider()->get(CuServices::Log))))
            log->write(QString("QuNumericControlW [" + objectName() + "]").toStdString(), msg.toStdString(), CuLog::LevelError, CuLog::CategoryWrite);
        else if(!cumbia) {
            perr("QuNumericControlW.onUpdate: cannot get a reference to cumbia either from context or CumbiaPool with target \"%s\"", da["src"].toString().c_str());
        }

        d->context->getLinkStats()->addError(msg.toStdString());
    }
    else if(d->auto_configure && is_config)
    {
        QString desc = "";
        if(da["dfs"] == "scalar" && da["writable"].toInt() > 0)
        {
            /* first apply format, if - correctly - specified */
            CuVariant m, M;
            m = da["min"];
            M = da["max"];
            std::string print_format = da["format"].toString();
            double min, max;
            bool ok;
            ok = m.to<double>(min);
            if(ok)
                ok = M.to<double>(max);

            if(!ok)
                min = max = 0.0f; // configureNumber will use format only

            NumberDelegate nd;
            nd.configureNumber(min, max, print_format.c_str());
            /* integerDigits() and decimalDigits() from NumberDelegate */
            setIntDigits(nd.integerDigits());
            setDecDigits(nd.decimalDigits());

            if(ok) {
                setMaximum(max);
                setMinimum(min);
            }
            else

            desc = "\n(min: "+ QString("%1").arg(min) + " max: "+ QString("%1").arg(max) + ")";

            if(!ok)
                pinfo("QuNumericControlW: maximum and minimum values not available for \"%s\", object \"%s\": "
                      "setting number of int and dec digits from format \"%s\"", qstoc(target()), qstoc(objectName()), print_format.c_str());

            /* can set current values instead */
            double val;
            bool can_be_double = da["w_value"].to<double>(val);
            if (can_be_double) {
                setValue(val);
            }
            if(!da["description"].isNull()) {
                desc.prepend(QString::fromStdString(da["description"].toString()));
            }
            setWhatsThis(desc);
            // save fetching configuration at every execute
            CuControlsWriterA *w = d->context->getWriter();
            if(w)
                w->saveConfiguration(da);
        }
        else
            perr("QuNumericControlW [%s]: invalid data format \"%s\" or read only source (writable: %d)", qstoc(objectName()),
                 da["dfs"].toString().c_str(), da["writable"].toInt());

    }
    setToolTip(msg);
}

/*! \brief returns a pointer to the context
 *
 * @return a pointer to the CuContext
 */
CuContext *QuNumericControlW::getContext() const
{
    return d->context;
}

