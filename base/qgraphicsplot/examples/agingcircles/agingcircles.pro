######################################################################
# Automatically generated by qmake (2.01a) Fri Mar 16 11:25:33 2012
######################################################################

TEMPLATE = app
TARGET = agingcircles
DEPENDPATH += .
INCLUDEPATH += . ../../src ../../src/curve ../../src/axes

QMAKE_CXXFLAGS += -O2

# Input
HEADERS += \
    agingcircles.h
FORMS += \
    agingcircles.ui
SOURCES += main.cpp \
    agingcircles.cpp

LIBS += -L../.. -lQGraphicsPlot
