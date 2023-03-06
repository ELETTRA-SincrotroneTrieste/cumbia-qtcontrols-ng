#include "cumbia-qtcontrols-ng-controls-designer.h"

/* Qu widgets includes */
#include "cupluginloader.h"
#include <QPluginLoader>
#include <cuformulaplugininterface.h>

#ifdef QUMBIA_EPICS_CONTROLS
#include <cuepics-world.h>
#include <cumbiaepics.h>
#include <cuepactionfactories.h>
#include <cuepcontrolsreader.h>
#include <cuepcontrolswriter.h>
#endif

#include <cumbiatango.h>
#include <cumacros.h>
#include <cuthreadfactoryimpl.h>
#include <qthreadseventbridgefactory.h>
#include <cutcontrolsreader.h>
#include <cutcontrolswriter.h>
#include <cutangoactionfactories.h>
#include <cutango-world.h>
#include <culog.h>
#include <qulogimpl.h>
#include <cuservices.h>
#include <cuserviceprovider.h>
#include <cumbiapool.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>
#include <QMimeData>
#include <QAction>
#include <quapps.h>
#include <QScrollArea>

#include <qunumericcontrol_w.h>

#define SOURCE_REGEXP "(([A-Za-z_0-9\\.]*[:]{1}[0-9]+[/])?(([A-Za-z_0-9\\.]+/[A-Za-z_0-9\\.]+/[A-Za-z_0-9\\.]+([/]{1,1}|[->]{2,2})[A-Za-z_0-9\\.]+)|(\\$[1-9]*([/]{1,1}|[->]{2})[A-Za-z_0-9\\.]+)){1}([\\(]{1}[&A-Za-z_0-9\\.,]+[\\)]{1})?[;]?){1,}"


DropEventFilter::DropEventFilter(QObject *parent) : QObject(parent)
{

}

bool DropEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::DragEnter)
    {
        QDragEnterEvent *dev = static_cast<QDragEnterEvent *>(event);
        dev->acceptProposedAction();
        return true; /* stop handling event here */
    }
    else if(event->type() == QEvent::Drop)
    {
        QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
        const QMimeData *mimeData = dropEvent->mimeData();
        QString point = mimeData->text();

        if(point.contains(QRegExp(SOURCE_REGEXP)) )
        {
            QDesignerFormWindowInterface *formWindow = 0;
            formWindow = QDesignerFormWindowInterface::findFormWindow(obj);
            if(formWindow)
            {
                formWindow->clearSelection(true);
                formWindow->selectWidget(qobject_cast<QWidget *>(obj));

                PointEditor pointEditor(qobject_cast<QWidget *>(obj), point);

                if(obj->metaObject()->indexOfProperty("source") > -1) {
                    pointEditor.textLabel()->setText("Set <b>source</b> point on " + obj->objectName());
                    pointEditor.setWindowTitle(QString("%1 source editor").arg(obj->objectName()));

                    pointEditor.exec();
                    if(pointEditor.result() == QDialog::Accepted)
                        formWindow->cursor()->setProperty("source", pointEditor.point());
                }

            }
            else
                perr("formWindow is null (\"%s\")", qstoc(obj->objectName()));
        }
        else
            perr("point is empty (cannot set \"%s\" on \"%s\")", qstoc(point), qstoc(obj->objectName()));

        return true; /* always eat drop events */
    }
    return false;
}

CuCustomWidgetInterface::CuCustomWidgetInterface(QObject *parent,
                                                 CumbiaPool *cumbia_p, const CuControlsFactoryPool &ctrl_factory_p)
    : QObject(parent)
{
    d_isInitialized = false;
    cumbia_pool = cumbia_p;
    m_ctrl_factory_pool = ctrl_factory_p;
}

CuCustomWidgetInterface::~CuCustomWidgetInterface()
{

}

void CuCustomWidgetInterface::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (d_isInitialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    if (manager)
        manager->registerExtensions(new TaskMenuFactory(manager), Q_TYPEID(QDesignerTaskMenuExtension));
    d_isInitialized = true;
}


CuCustomWidgetCollectionInterface::CuCustomWidgetCollectionInterface(QObject *parent): QObject(parent)
{
    printf("\e[1;32mo \e[0m cumbia-qtcontrols-ng [\e[1;33mcontrols\e[1;32m]  %p\e[0m\n", this);
    cumbia_pool = new CumbiaPool();
    CuModuleLoader loader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    CuPluginLoader plulo;
    QString plupath = plulo.getPluginAbsoluteFilePath(CUMBIA_QTCONTROLS_PLUGIN_DIR, "cuformula-plugin.so");
    QPluginLoader pluginLoader(plupath);
    QObject *plugin = pluginLoader.instance();
    if (plugin){
        CuFormulaPluginI *fplu = qobject_cast<CuFormulaPluginI *>(plugin);
        if(!fplu)
            perr("Failed to load formula plugin");
        else {
            fplu->initialize(cumbia_pool, m_ctrl_factory_pool);
        }
    }
    else {
        perr("failed to load plugin loader under path %s: %s", qstoc(plupath), qstoc(pluginLoader.errorString()));
    }

    d_plugins.append(new QuNumericControlW_Interface(this, cumbia_pool, m_ctrl_factory_pool));
}

CuCustomWidgetCollectionInterface::~CuCustomWidgetCollectionInterface()
{
    printf("\e[1;31mo\e[0m ~CuCustomWidgetCollectionInterface %p\n", this);
    printf("\e[1;31m-\e[0m  not releasing cumbia tango resources:\n");
    printf("\e[1;31m-\e[0m  https://github.com/tango-controls/cppTango/issues/540 \n");
    cumbia_free();
}

QList<QDesignerCustomWidgetInterface*> CuCustomWidgetCollectionInterface::customWidgets(void) const
{
    return d_plugins;
}

/**
 * @brief CuCustomWidgetCollectionInterface::cumbia_tango_free free cumbia tango resources
 *
 * \par Note
 * This is a slot that can be invoked to free Tango resources *before* the class destruction time.
 * If we delete Tango device proxies in the CuCustomWidgetCollectionInterface class destructor we
 * may run into a crash ( https://github.com/tango-controls/cppTango/issues/540 )
 *
 * For example, the *cumparsita* application, uses QMetaObject::invokeMethod to call this method
 * *before* ~CuCustomWidgetCollectionInterface is invoked.
 *
 */
void CuCustomWidgetCollectionInterface::cumbia_tango_free()
{
    Cumbia* c = cumbia_pool->get("tango");
    if(c) {
        printf("\e[1;31m-\e[0m ~cumbia_tango %p\n", c);
        delete c;
    }
}

void CuCustomWidgetCollectionInterface::cumbia_free()
{
#ifdef CUMBIA_EPICS
    Cumbia* c = cumbia_pool->get("epics");
    if(c)
    {
        printf("\e[1;31m-\e[0m ~cumbia_epics %p\n", c);
        cumbia_pool->unregisterCumbiaImpl("epics");
        delete c;
    }
#endif

    // plugins are destroyed
    // cumbia-formula is destroyed by the cuformula plugin destructor
    if(cumbia_pool)
    {
        printf("\e[1;31m-\e[0m ~cumbia_pool %p\n", cumbia_pool);
        delete cumbia_pool;
    }
}

/* TaskMenuFactory */
TaskMenuFactory::TaskMenuFactory(QExtensionManager *parent): QExtensionFactory(parent) {
}

QObject *TaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const {
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;
    qDebug() << __PRETTY_FUNCTION__ << object << iid << "parent" << parent;
    return new TaskMenuExtension((QWidget*) object, parent);
}
/* */

/* TaskMenuExtension */
TaskMenuExtension::TaskMenuExtension(QWidget *widget, QObject *parent): QObject(parent),
    d_cuanw(widget), d_editConnectionAction(0), m_pointEditor(0)
{
    d_editConnectionAction = new QAction(tr("Edit Connection..."), this);
    connect(d_editConnectionAction, SIGNAL(triggered()), this, SLOT(editConnection()));
}

QList<QAction *> TaskMenuExtension::taskActions() const
{
    QList<QAction *> list;
    list.append(d_editConnectionAction);
    return list;
}

QAction *TaskMenuExtension::preferredEditAction() const {
    return d_editConnectionAction;
}

void TaskMenuExtension::editConnection() {
    QString src;
    src = d_cuanw->property("target").toString();
    if (!m_pointEditor)
        m_pointEditor = new PointEditor(nullptr, src);
    if (m_pointEditor->exec() == QDialog::Accepted) {
        QDesignerFormWindowInterface *formWindow = 0;
        formWindow = QDesignerFormWindowInterface::findFormWindow(d_cuanw);
        formWindow->cursor()->setProperty("target", m_pointEditor->point());
    }
}


QuNumericControlW_Interface::QuNumericControlW_Interface(QObject* parent, CumbiaPool *cumbia_p, const CuControlsFactoryPool& ctrl_factory_p)
    : CuCustomWidgetInterface(parent, cumbia_p, ctrl_factory_p)
{
    d_name = "QuNumericControlW";
    d_include = "qunumericcontrol_w.h";
    d_icon = QPixmap(":pixmaps/qunumericcontrolw.png");
    d_domXml =
            "<widget class=\"QuNumericControlW\" name=\"quNumericCtrlW\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>112</width>\n"
            "   <height>42</height>\n"
            "  </rect>\n"
            " </property>\n"
            "</widget>\n";
}

QWidget* QuNumericControlW_Interface::createWidget(QWidget* parent)
{
    QuNumericControlW *nw = new QuNumericControlW(parent, cumbia_pool, m_ctrl_factory_pool);
    DropEventFilter *dropEventFilter = new DropEventFilter(nw);
    nw->installEventFilter(dropEventFilter);
    return nw;

}
