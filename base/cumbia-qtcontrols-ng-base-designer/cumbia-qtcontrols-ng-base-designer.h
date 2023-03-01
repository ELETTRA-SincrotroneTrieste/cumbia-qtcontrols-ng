#ifndef QUTANGOCONTROLSPLUGINS_H
#define QUTANGOCONTROLSPLUGINS_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>

class CumbiaPool;
class CuControlsFactoryPool;
class EditSourceDialog;

class CuCustomWidgetInterface: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    CuCustomWidgetInterface(QObject *parent);

    virtual bool isContainer() const { return false; }
    virtual bool isInitialized() const { return d_isInitialized; }
    virtual QIcon icon() const { return d_icon; }
    virtual QString codeTemplate() const { return d_codeTemplate; }
    virtual QString domXml() const { return d_domXml; }
    virtual QString group() const { return "cumbia-qtcontrols-ng-base"; }
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
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> d_plugins;
};

class QuNumericW_Interface : public CuCustomWidgetInterface
{
   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

  public:
    QuNumericW_Interface(QObject* parent);
    virtual QWidget* createWidget(QWidget* parent);
};


#endif // QUTANGOCONTROLSPLUGINS_H
