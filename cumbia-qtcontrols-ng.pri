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

LIBS += -L$${INSTALL_ROOT}/lib -l$${cumbia_qtcontrols_ng_LIB}

DISTFILES += \
    $$PWD/cumbia-qtcontrols-ng.md

