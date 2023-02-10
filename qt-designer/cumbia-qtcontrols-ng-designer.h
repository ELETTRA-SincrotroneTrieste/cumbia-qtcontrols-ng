#ifndef QUTANGOCONTROLSPLUGINS_H
#define QUTANGOCONTROLSPLUGINS_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>

#include "pointeditor.h"

#include <cucontrolsfactorypool.h>
#include <qulogimpl.h>

class CumbiaPool;
class CuControlsFactoryPool;
class EditSourceDialog;

/** \brief intercepts DropEvents and pops up a Dialog to configure the source
  *        or the target of the object.
  *
  * Just drag and drop an attribute or a command from jive or from any other QTango panel
  * into any of the QTango widgets exported by the Qt designer and quickly configure its
  * source or target.
  */
class DropEventFilter : public QObject
{
    Q_OBJECT
public:
    DropEventFilter(QObject *parent);

 protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

class CuCustomWidgetInterface: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    CuCustomWidgetInterface(QObject *parent, CumbiaPool *cumbia_p, const CuControlsFactoryPool &ctrl_factory_p);

    ~CuCustomWidgetInterface();

    virtual bool isContainer() const { return false; }
    virtual bool isInitialized() const { return d_isInitialized; }
    virtual QIcon icon() const { return d_icon; }
    virtual QString codeTemplate() const { return d_codeTemplate; }
    virtual QString domXml() const { return d_domXml; }
    virtual QString group() const { return "cumbia-qtcontrols-ng"; }
    virtual QString includeFile() const { return d_include; }
    virtual QString name() const { return d_name; }
    virtual QString toolTip() const { return d_toolTip; }
    virtual QString whatsThis() const { return d_whatsThis; }
    virtual void initialize(QDesignerFormEditorInterface *);

protected:
    QString d_name;
    QString d_include;
    QString d_toolTip;
    QString d_whatsThis;
    QString d_domXml;
    QString d_codeTemplate;
    QIcon d_icon;
    CumbiaPool *cumbia_pool;
    CuControlsFactoryPool m_ctrl_factory_pool;

private:
    bool d_isInitialized;
};


#include <tango/tango.h>

class CuCustomWidgetCollectionInterface:
        public QObject,
        public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif

    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    CuCustomWidgetCollectionInterface(QObject *parent = nullptr);
    ~CuCustomWidgetCollectionInterface();

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

public slots:
    void cumbia_tango_free();
    void cumbia_free();

private:
    QList<QDesignerCustomWidgetInterface*> d_plugins;

    CumbiaPool *cumbia_pool;
    CuControlsFactoryPool m_ctrl_factory_pool;
    QuLogImpl m_log_impl;
};

class QuApplyNumericWidgetInterface : public CuCustomWidgetInterface
{
   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

  public:
    QuApplyNumericWidgetInterface(QObject* parent, CumbiaPool *cumbia_p, const CuControlsFactoryPool &ctrl_factory_p);
    virtual QWidget* createWidget(QWidget* parent);
};

class TaskMenuFactory: public QExtensionFactory
{
Q_OBJECT
public:
    TaskMenuFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};

class TaskMenuExtension: public QObject, public QDesignerTaskMenuExtension
{
Q_OBJECT
Q_INTERFACES(QDesignerTaskMenuExtension)

public:
    TaskMenuExtension(QWidget *widget, QObject *parent);

    QAction *preferredEditAction() const;
    QList<QAction *> taskActions() const;

private slots:
    void editConnection();
    void editAttributes();

protected:
    QWidget *d_cuanw;
    QAction *d_editConnectionAction;
    void setupSourceTargetDialog(QWidget *s);

    PointEditor *m_pointEditor;
};

#endif // QUTANGOCONTROLSPLUGINS_H
