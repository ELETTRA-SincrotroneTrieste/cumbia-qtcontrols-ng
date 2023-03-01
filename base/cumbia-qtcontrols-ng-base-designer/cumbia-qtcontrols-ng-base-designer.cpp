#include "cumbia-qtcontrols-ng-base-designer.h"

/* Qu widgets includes */
#include <QPluginLoader>

#include <cumacros.h>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>
#include <qunumericwidget.h>

CuCustomWidgetInterface::CuCustomWidgetInterface(QObject *parent)
    : QObject(parent)
{
    d_isInitialized = false;
}

void CuCustomWidgetInterface::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (d_isInitialized)
        return;
    d_isInitialized = true;
}

CuCustomWidgetCollectionInterface::CuCustomWidgetCollectionInterface(QObject *parent): QObject(parent) {
    d_plugins.append(new QuNumericW_Interface(this));
}

QList<QDesignerCustomWidgetInterface*> CuCustomWidgetCollectionInterface::customWidgets(void) const
{
    return d_plugins;
}

QuNumericW_Interface::QuNumericW_Interface(QObject* parent)
    : CuCustomWidgetInterface(parent)
{
    d_name = "QuNumericW";
    d_include = "qunumericwidget.h";
    d_icon = QPixmap(":pixmaps/qunumericwidget.png");
    d_domXml =
            "<widget class=\"QuNumericW\" name=\"quNumericW\">\n"
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

QWidget* QuNumericW_Interface::createWidget(QWidget* parent) {
    return new QuNumericW(parent);
}
