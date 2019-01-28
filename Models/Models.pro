QT       -= gui
QT       += network

TARGET = QTwitchModels
TEMPLATE = lib
include(../common.pri)

DEFINES += QTWITCH_LIBRARY

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
    abstractentitledimagesmodel.cpp \
    helixscrollablemodel.cpp \
    legacyscrollablemodel.cpp \
    topgamesmodel.cpp \
    streamsmodel.cpp \
    followedchannelsmodel.cpp \
    gamessearchmodel.cpp \
    channelssearchmodel.cpp \
    streamssearchmodel.cpp

HEADERS += \
    abstractentitledimagesmodel.h \
    entitledimage.h \
    helixscrollablemodel.h \
    legacyscrollablemodel.h \
    topgamesmodel.h \
    streamsmodel.h \
    followedchannelsmodel.h \
    gamessearchmodel.h \
    channelssearchmodel.h \
    streamssearchmodel.h \
    payloads.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

pvs_studio.output = true
pvs_studio.sources = $${SOURCES}
include(../PVS-Studio.pri)
