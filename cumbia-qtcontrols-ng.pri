# + ----------------------------------------------------------------- +
#
# Customization section:
#
# Customize the following paths according to your installation:
#
#
# Here cumbia-qtcontrols-ng will be installed
# INSTALL_ROOT can be specified from the command line running qmake "INSTALL_ROOT=/my/install/path"
#

isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local/cumbia-libs
}

# INSTALL_ROOT is used to install the target
# prefix is used within DEFINES +=
#
# cumbia installation script uses a temporary INSTALL_ROOT during build
# and then files are copied into the destination prefix. That's where
# configuration files must be found by the application when the script
# installs everything at destination
#
isEmpty(prefix) {
    prefix = $${INSTALL_ROOT}
}
#
#
# Here cumbia-qtcontrols-ng include files will be installed
    CUMBIA_QTCONTROLS_NG_INCLUDES=$${INSTALL_ROOT}/include/cumbia-qtcontrols-ng
#
#
# Here cumbia-qtcontrols-ng share files will be installed
#
    CUMBIA_QTCONTROLS_NG_SHARE=$${INSTALL_ROOT}/share/cumbia-qtcontrols-ng
#
#
# Here cumbia-qtcontrols-ng libraries will be installed
    CUMBIA_QTCONTROLS_NG_LIBDIR=$${INSTALL_ROOT}/lib
#
#
# Here cumbia-qtcontrols-ng documentation will be installed
    CUMBIA_QTCONTROLS_NG_DOCDIR=$${INSTALL_ROOT}/share/doc/cumbia-qtcontrols-ng
#
# The name of the library
    cumbia_qtcontrols_ng_LIB=cumbia-qtcontrols-ng
#
#


MOC_DIR=moc
OBJECTS_DIR=objs


CONFIG += link_pkgconfig
PKGCONFIG += cumbia cumbia-qtcontrols

packagesExist(qwt){
    PKGCONFIG += qwt
}
# qwt for qt6
else:packagesExist(Qt6Qwt6){
    PKGCONFIG += Qt6Qwt6
    message("qwt6 for QT6")
}
else:packagesExist(Qt5Qwt6){
    PKGCONFIG += Qt5Qwt6
}  else {
    warning("cumbia-qtcontrols.pri: no pkg-config file found for either qwt or Qt5Qwt6")
    warning("cumbia-qtcontrols.pri: export PKG_CONFIG_PATH=/usr/path/to/qwt/lib/pkgconfig if you want to enable pkg-config for qwt")
    warning("cumbia-qtcontrols.pri: if you build and install qwt from sources, be sure to uncomment/enable ")
    warning("cumbia-qtcontrols.pri: QWT_CONFIG     += QwtPkgConfig in qwtconfig.pri qwt project configuration file")
}


QT       += widgets svg opengl

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += openglwidgets
}

LIBS += -L$${INSTALL_ROOT}/lib -l$${cumbia_qtcontrols_ng_LIB}

DISTFILES += \
    $$PWD/cumbia-qtcontrols-ng.md

