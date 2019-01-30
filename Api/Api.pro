QT       += network
QT       -= gui

TARGET = QTwitchApi
TEMPLATE = lib
include(../common.pri)

CONFIG += object_parallel_to_source

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
    client.cpp \
    request.cpp \
    Helix/endpoints.cpp \
    v5/endpoints.cpp \
    Usher/endpoints.cpp \
    qsettingscredentialsstorage.cpp \
    authorizationflow.cpp

HEADERS += \
    client.h \
    request.h \
    response.h \
    jsonsetter.h \
    credentials.h \
    Helix/endpoints.h \
    Helix/data.h \
    v5/endpoints.h \
    v5/data.h \
    Usher/endpoints.h \
    Usher/data.h \
    qsettingscredentialsstorage.h \
    authorizationflow.h

unix {
    isEmpty(INSTALL_PREFIX): INSTALL_PREFIX = /usr
    target.path = $$INSTALL_PREFIX/lib
    INSTALLS += target
}

pvs_studio.output = true
pvs_studio.sources = $${SOURCES}
include(../PVS-Studio.pri)
