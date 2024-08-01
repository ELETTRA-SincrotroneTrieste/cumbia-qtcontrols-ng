#include "cumbia-qtcontrols-ng-base-designer.h"

/* Qu widgets includes */
#include <QPluginLoader>

#include <cumacros.h>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>
#include <qunumericwidget.h>
#include <quarrayplot.h>
#include <quscalarplot.h>
#include <qucircularplotwidget.h>

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
    d_plugins.append(new QuScalarPlot_Interface(this));
    d_plugins.append(new QuArrayPlot_Interface(this));
    d_plugins.append(new QuCircularPlot_Interface(this));
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
    d_icon = QPixmap(":pixmaps/qunumericcontrolw.png");
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

QuScalarPlot_Interface::QuScalarPlot_Interface(QObject* parent)
    : CuCustomWidgetInterface(parent)
{
    d_name = "QuScalarPlot";
    d_include = "quscalarplot.h";
    d_icon = QPixmap(":pixmaps/quscalarplot.png");
    d_domXml =
        "<widget class=\"QuScalarPlot\" name=\"quScalarPlot\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>400</width>\n"
        "   <height>340</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QWidget* QuScalarPlot_Interface::createWidget(QWidget* parent) {
    return new QuScalarPlot(parent);
}

QuArrayPlot_Interface::QuArrayPlot_Interface(QObject* parent)
    : CuCustomWidgetInterface(parent)
{
    d_name = "QuArrayPlot";
    d_include = "quarrayplot.h";
    d_icon = QPixmap(":pixmaps/quarrayplot.png");
    d_domXml =
        "<widget class=\"QuArrayPlot\" name=\"quArrayPlot\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>400</width>\n"
        "   <height>340</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QWidget* QuArrayPlot_Interface::createWidget(QWidget* parent) {
    return new QuArrayPlot(parent);
}

QuCircularPlot_Interface::QuCircularPlot_Interface(QObject* parent)
    : CuCustomWidgetInterface(parent)
{
    d_name = "QuCircularPlotW";
    d_include = "qucircularplotwidget.h";
    d_icon = QPixmap(":pixmaps/qucircularplot.png");
    d_domXml =
        "<widget class=\"QuCircularPlotW\" name=\"quCircularPlot\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>300</width>\n"
        "   <height>300</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QWidget* QuCircularPlot_Interface::createWidget(QWidget* parent) {
    return new QuCircularPlotW(parent);
}
