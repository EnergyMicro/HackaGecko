TEMPLATE = app
INCLUDEPATH += .

include(./embedded-widgets-1.1.0/src/common/common.pri)
include(./embedded-widgets-1.1.0/src/basicgraph/basicgraph.pri)

CONFIG += release
CONFIG -= debug

RESOURCES += patientcare.qrc

DEFINES          += WINVER=0x0501 # needed for mingw to pull in appropriate dbt business...probably a better way to do this
LIBS             += -lsetupapi

HEADERS += \
    mainwindow.h \
    datagenerator.h \
    patientcarecontroller.h \
    lazytimer.h \
    PortListener.h \
    qextserialport.h \
    qextserialenumerator.h \
    qextserialport_global.h \
    datafilter.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    datagenerator.cpp \
    patientcarecontroller.cpp \
    lazytimer.cpp \
    PortListener.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialenumerator_win.cpp \
    datafilter.cpp

FORMS = patientcare.ui

QT *= svg
