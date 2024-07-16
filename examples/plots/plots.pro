isEmpty(CUMBIA_ROOT) {
    CUMBIA_ROOT=/usr/local/cumbia-libs
}
include($${CUMBIA_ROOT}/include/quapps/quapps.pri)

PKGCONFIG += cumbia-qtcontrols-ng

# CONFIG += debug | release

DEFINES += QT_NO_DEBUG_OUTPUT

# RESOURCES +=

SOURCES += src/main.cpp \
    src/plots.cpp

HEADERS += \
    src/plots.h

# cuuimake runs uic
# FORMS    = src/arrayplot.ui
# but we need to include ui_xxxx.h file amongst the headers
# in order to be recompiled when it changes
#
HEADERS += \
    ui/ui_arrayplot.h

# - ui: where to find cuuimake ui_*.h files
#   since FORMS is not used
# - src: where to find headers included by
#   ui_*.h (e.g. for custom widget promoted
#   from the Qt designer)
#
INCLUDEPATH += ui src

TARGET = arrayplot

!wasm-emscripten {
    TARGET   = bin/$${TARGET}
} else {
    TARGET = wasm/$${TARGET}
}

#
# make install works if INSTALL_DIR is given to qmake
#
!isEmpty(INSTALL_DIR) {
    wasm-emscripten {
        inst.files = wasm/*
    } else {
        inst.files = $${TARGET}
    }
    inst.path = $${INSTALL_DIR}
    INSTALLS += inst
    message("-")
    message("INSTALLATION")
    message("       execute `make install` to install 'arrayplot' under $${INSTALL_DIR} ")
    message("-")
} else {
    message("-")
    message("INSTALLATION")
    message("       call qmake INSTALL_DIR=/usr/local/bin to install arrayplot later with `make install` ")
    message("-")
}

# unix:LIBS += -L. -lmylib

# unix:INCLUDEPATH +=  . ../../src

