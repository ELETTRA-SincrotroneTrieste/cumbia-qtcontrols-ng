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

unix:!android-g++ {
    CONFIG += link_pkgconfig
    PKGCONFIG += cumbia cumbia-qtcontrols
}

QT       += widgets svg opengl

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += openglwidgets
}

LIBS += -L$${INSTALL_ROOT}/lib -l$${cumbia_qtcontrols_ng_LIB}

DISTFILES += \
    $$PWD/cumbia-qtcontrols-ng.md

