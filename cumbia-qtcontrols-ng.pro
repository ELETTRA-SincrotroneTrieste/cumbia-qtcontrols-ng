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

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/circularplot/qucircularplotdisksector.cpp \
    base/circularplot/qucircularplotselectionvalue.cpp \
    base/circularplot/out_of_bounds_distortions.cpp \
    base/circularplot/qucircularplotattributes.cpp \
    base/circularplot/qucircularplotconfigurator.cpp \
    base/circularplot/qucircularplotcurve.cpp \
    base/circularplot/qucircularplotcurveselectionevents.cpp \
    base/circularplot/qucircularplotengine.cpp \
    base/circularplot/quzoomer.cpp \
    base/circularplot/quzoomevents.cpp \
    base/circularplot/qucircularplotwidget.cpp \
    base/circularplot/qucircularplotitem.cpp \
    controls/numeric/qunumericcontrol_w.cpp \
    base/numeric/qunumericclickrepeater.cpp \
    base/numeric/qunumericengine.cpp \
    base/numeric/qunumericwidget.cpp \
    base/numeric/qunumericitem.cpp


HEADERS += \
    base/circularplot/qucircularplot.h \
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
    controls/numeric/qunumericcontrol_w.h

DISTFILES += \
    Doxyfile

unix {
    doc.commands = \
    doxygen \
    Doxyfile;

    doc.files = doc/*
    doc.path = $${CUMBIA_QTCONTROLS_NG_DOCDIR}
    QMAKE_EXTRA_TARGETS += doc

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

INCLUDEPATH += base controls base/numeric base/circularplot controls/numeric

# remove ourselves from -l (.pri)
LIBS -= -l$${cumbia_qtcontrols_ng_LIB}

# RESOURCES += \
#    cumbia-qtcontrols-ng.qrc
