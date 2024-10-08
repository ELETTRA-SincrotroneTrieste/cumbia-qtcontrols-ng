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


### VERSION
###
#
VERSION_HEX = 0x020201
VERSION = 2.2.1
#
###

DEFINES += CUMBIA_QTCONTROLS_NG_VERSION_STR=\"\\\"$${VERSION}\\\"\" \
    CUMBIA_QTCONTROLS_NG_VERSION=$${VERSION_HEX}

MOC_DIR=moc
OBJECTS_DIR=objs


exists($${INSTALL_ROOT}/include/cumbia) {
    INCLUDEPATH += $${INSTALL_ROOT}/include/cumbia
    LIBS += -L$${INSTALL_ROOT}/lib -lcumbia
} else {
    error("cumbia installation not found under $${INSTALL_ROOT}")
}
# libs at this point contain $${INSTALL_ROOT}/lib

exists($${INSTALL_ROOT}/include/cumbia-qtcontrols/cumbia-qtcontrols.pri) {
    INCLUDEPATH += $${INSTALL_ROOT}/include/cumbia-qtcontrols
    LIBS += -lcumbia-qtcontrols
} else {
    error("cumbia-qtcontrols installation not found under $${INSTALL_ROOT}")
}



CONFIG += link_pkgconfig
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

