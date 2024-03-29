#-------------------------------------------------
#
# Project created by QtCreator 2019-05-09T16:46:14
#
#-------------------------------------------------

include(cumbia-qtcontrols-ng.pri)

CONFIG += debug

VERSION_HEX = 0x0100000
VERSION = 1.1.0

TARGET = $${cumbia_qtcontrols_ng_LIB}
TEMPLATE = lib

DEFINES -= QT_NO_DEBUG_OUTPUT

DEFINES += CUMBIA_QTCONTROLS_NG_LIBRARY

DEFINES += CUMBIA_QTCONTROLS_NG_VERSION_STR=\"\\\"$${VERSION}\\\"\" \
    CUMBIA_QTCONTROLS_NG_VERSION=$${VERSION_HEX} \
    VER_MAJ=$${VER_MAJ} \
    VER_MIN=$${VER_MIN} \
    VER_FIX=$${VER_FIX} \
    SHAREDIR=\"\\\"$${SHAREDIR}\\\"\"  \
    DOCDIR=\"\\\"$${DOC_DIR}\\\"\"  \
    INSTALLROOT=\"\\\"$${INSTALL_ROOT}\\\"\"  \
    LIBDIR=\"\\\"$${LIBDIR}\\\"\"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# change the place where you want the documentation and other files to be installed
SHAREDIR = $${INSTALL_ROOT}/share

SOURCES += \
    base/circularplot/qucircularplotdisksector.cpp \
    base/circularplot/qucircularplotselectionvalue.cpp \
    base/circularplot/out_of_bounds_distortions.cpp \
    base/circularplot/qucircularplotconfigurator.cpp \
    base/circularplot/qucircularplotcurve.cpp \
    base/circularplot/qucircularplotcurveselectionevents.cpp \
    base/circularplot/qucircularplotengine.cpp \
    base/circularplot/quzoomer.cpp \
    base/circularplot/quzoomevents.cpp \
    base/circularplot/qucircularplotwidget.cpp \
    base/circularplot/qucircularplotitem.cpp \
    \
    base/qgraphicsplot/axes/axesmanager.cpp \
    base/qgraphicsplot/axes/axiscouple.cpp \
    base/qgraphicsplot/axes/scaleitem.cpp \
    base/qgraphicsplot/axes/scaleitemprivate.cpp \
    base/qgraphicsplot/colorpalette.cpp \
    base/qgraphicsplot/curve/curveitem.cpp \
    base/qgraphicsplot/curve/curveitemprivate.cpp \
    base/qgraphicsplot/curve/data.cpp \
    base/qgraphicsplot/curve/itempositionchangelistener.cpp \
    base/qgraphicsplot/curve/painters/circleitemset.cpp \
    base/qgraphicsplot/curve/painters/dotspainter.cpp \
    base/qgraphicsplot/curve/painters/histogrampainter.cpp \
    base/qgraphicsplot/curve/painters/histogrampainterprivate.cpp \
    base/qgraphicsplot/curve/painters/linepainter.cpp \
    base/qgraphicsplot/curve/painters/stepspainter.cpp \
    base/qgraphicsplot/curve/painters/stepspainterprivate.cpp \
    base/qgraphicsplot/curve/point.cpp \
    base/qgraphicsplot/curve/pointdata.cpp \
    base/qgraphicsplot/curve/pointprivate.cpp \
    base/qgraphicsplot/curve/scenecurve.cpp \
    base/qgraphicsplot/externalscalewidget.cpp \
    base/qgraphicsplot/horizontalscalewidget.cpp \
    base/qgraphicsplot/items/itemmovelistener.cpp \
    base/qgraphicsplot/items/legenditem.cpp \
    base/qgraphicsplot/items/markeritem.cpp \
    base/qgraphicsplot/items/markeritemprivate.cpp \
    base/qgraphicsplot/items/targetitem.cpp \
    base/qgraphicsplot/items/targetitemprivate.cpp \
    base/qgraphicsplot/plotsaver/plotscenewidgetsaver.cpp \
    base/qgraphicsplot/plotscenewidget.cpp \
    base/qgraphicsplot/plugins/qgraphicsplotplugin.cpp \
    base/qgraphicsplot/plugins/qrc_qgraphicsplotplugin.cpp \
    base/qgraphicsplot/properties/propertydialog.cpp \
    base/qgraphicsplot/properties/settingsloader.cpp \
    base/qgraphicsplot/qgraphicsplotitem.cpp \
    base/qgraphicsplot/qgraphicsplotitem_private.cpp \
    base/qgraphicsplot/qgraphicszoomer.cpp \
    base/qgraphicsplot/scalelabelinterface.cpp \
    base/qgraphicsplot/scalelabels/timescalelabel.cpp \
    base/qgraphicsplot/verticalscalewidget.cpp \
    \
    controls/numeric/qunumericcontrol_w.cpp \
    base/numeric/qunumericclickrepeater.cpp \
    base/numeric/qunumericengine.cpp \
    base/numeric/qunumericwidget.cpp \
    base/numeric/qunumericitem.cpp


HEADERS += \
    base/circularplot/qucircularplotdisksector.h \
    base/circularplot/qucircularplotselectionvalue.h \
    base/circularplot/out_of_bounds_distortions.h \
    base/circularplot/qucircularplot_drawable_i.h \
    base/circularplot/qucircularplotattributes.h \
    base/circularplot/qucircularplotconfigurator.h \
    base/circularplot/qucircularplotcurve.h \
    base/circularplot/qucircularplotcurveselectionevents.h \
    base/circularplot/qucircularplotengine.h \
    base/circularplot/quzoomer.h \
    base/circularplot/quzoomevents.h \
    base/circularplot/qucircularplotwidget.h \
    base/circularplot/qucircularplotitem.h \
    base/numeric/qunumericclickrepeater.h \
    base/numeric/qunumericengine.h \
    base/numeric/qunumericitem.h \
    base/numeric/qunumericwidget.h \
    base/qgraphicsplot/axes/axesmanager.h \
    base/qgraphicsplot/axes/axiscouple.h \
    base/qgraphicsplot/axes/scaleitem.h \
    base/qgraphicsplot/axes/scaleitemprivate.h \
    base/qgraphicsplot/axischangelistener.h \
    base/qgraphicsplot/colorpalette.h \
    base/qgraphicsplot/colors.h \
    base/qgraphicsplot/curve/curvechangelistener.h \
    base/qgraphicsplot/curve/curveitem.h \
    base/qgraphicsplot/curve/curveitemprivate.h \
    base/qgraphicsplot/curve/data.h \
    base/qgraphicsplot/curve/itemmovelistener.h \
    base/qgraphicsplot/curve/itempainterinterface.h \
    base/qgraphicsplot/curve/itempositionchangelistener.h \
    base/qgraphicsplot/curve/painters/circleitemset.h \
    base/qgraphicsplot/curve/painters/dotspainter.h \
    base/qgraphicsplot/curve/painters/dotspainterprivate.h \
    base/qgraphicsplot/curve/painters/histogrampainter.h \
    base/qgraphicsplot/curve/painters/histogrampainterprivate.h \
    base/qgraphicsplot/curve/painters/linepainter.h \
    base/qgraphicsplot/curve/painters/linepainterprivate.h \
    base/qgraphicsplot/curve/painters/stepspainter.h \
    base/qgraphicsplot/curve/painters/stepspainterprivate.h \
    base/qgraphicsplot/curve/point.h \
    base/qgraphicsplot/curve/pointdata.h \
    base/qgraphicsplot/curve/pointprivate.h \
    base/qgraphicsplot/curve/scenecurve.h \
    base/qgraphicsplot/curve/scenecurveprivate.h \
    base/qgraphicsplot/externalscalewidget.h \
    base/qgraphicsplot/horizontalscalewidget.h \
    base/qgraphicsplot/items/itemmovelistener.h \
    base/qgraphicsplot/items/legenditem.h \
    base/qgraphicsplot/items/markeritem.h \
    base/qgraphicsplot/items/markeritemprivate.h \
    base/qgraphicsplot/items/targetitem.h \
    base/qgraphicsplot/items/targetitemprivate.h \
    base/qgraphicsplot/mouseeventlistener.h \
    base/qgraphicsplot/plotgeometryeventlistener.h \
    base/qgraphicsplot/plotsaver/plotscenewidgetsaver.h \
    base/qgraphicsplot/plotscenewidget.h \
    base/qgraphicsplot/plugins/qgraphicsplotplugin.h \
    base/qgraphicsplot/properties/propertydialog.h \
    base/qgraphicsplot/properties/settingsloader.h \
    base/qgraphicsplot/qgraphicsplotitem.h \
    base/qgraphicsplot/qgraphicsplotitem_private.h \
    base/qgraphicsplot/qgraphicsplotmacros.h \
    base/qgraphicsplot/qgraphicszoomer.h \
    base/qgraphicsplot/scalelabelinterface.h \
    base/qgraphicsplot/scalelabels/timescalelabel.h \
    base/qgraphicsplot/verticalscalewidget.h \
    base/qgraphicsplot/xyplotinterface.h \
    controls/numeric/qunumericcontrol_w.h

DISTFILES +=

unix {
    doc.commands = \
    doxygen \
    Doxyfile;

    doc.files = docs/*
    doc.path = $${CUMBIA_QTCONTROLS_NG_DOCDIR}
    QMAKE_EXTRA_TARGETS += doc
    QMAKE_CXXFLAGS += -std=c++17

    inc.files = $${HEADERS}
    inc.path = $${CUMBIA_QTCONTROLS_NG_INCLUDES}

    other_inst.files = $${DISTFILES}
    other_inst.path = $${CUMBIA_QTCONTROLS_NG_INCLUDES}

    target.path = $${CUMBIA_QTCONTROLS_NG_LIBDIR}
    INSTALLS += target inc other_inst

#message("=====================================")
#message("DOC INSTALL SKIPPED!!!!!!!!!!!!!!!!")
#message("=====================================")

    !android-g++ {
        INSTALLS += doc
    }
} # unix

# generate pkg config file
CONFIG += create_pc create_prl no_install_prl

    QMAKE_PKGCONFIG_NAME = $${cumbia_qtcontrols_ng_LIB}
    QMAKE_PKGCONFIG_DESCRIPTION = cumbia module for Qt SVG integration
    QMAKE_PKGCONFIG_PREFIX = $${INSTALL_ROOT}
    QMAKE_PKGCONFIG_LIBDIR = $${target.path}
    QMAKE_PKGCONFIG_INCDIR = $${inc.path}
    QMAKE_PKGCONFIG_VERSION = $${VERSION}
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig

INCLUDEPATH += base controls base/numeric base/circularplot controls/numeric \
    base/qgraphicsplot/axes base/qgraphicsplot/plotsaver base/qgraphicsplot/properties \
    base/qgraphicsplot/curve  base/qgraphicsplot/extscaleplotscenewidget \
    base/qgraphicsplot/items  base/qgraphicsplot/

# remove ourselves from -l (.pri)
LIBS -= -l$${cumbia_qtcontrols_ng_LIB}

# RESOURCES += \
#    cumbia-qtcontrols-ng.qrc


