#-------------------------------------------------
#
# Project created by QtCreator 2022-07-06T16:49:33
#
#-------------------------------------------------

QT       += core gui xml concurrent websockets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_LFLAGS += -no-pie

TARGET = AgvMonitor
TEMPLATE = app

CONFIG(debug,debug|release):DESTDIR = $$PWD/debug
CONFIG(release,debug|release):DESTDIR = $$PWD/bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

include(mapView/mapView.pri)
include(webSocket/webSocket.pri)
include(login/login.pri)
include(errorinfo/errorinfo.pri)
include(baseinfo/baseinfo.pri)
include(tasklist/tasklist.pri)
include(allinfo/allinfo.pri)
include(switch/switch.pri)
include(setting/setting.pri)
include(moduleData/moduleData.pri)
include(common/common.pri)

SOURCES += \
    main.cpp \
    widgetmain.cpp \
    widgethomepage.cpp \
    widgetapp.cpp

HEADERS += \
    customData.h \
    widgetmain.h \
    widgethomepage.h \
    widgetapp.h

FORMS += \
    login/widgetlogin.ui \
    widgetmain.ui \
    widgethomepage.ui \
    setting/widgetsetting.ui \
    errorinfo/widgeterror.ui \
    baseinfo/widgetbaseinfo.ui \
    errorinfo/widgeterrorinfo.ui \
    tasklist/widgettasklist.ui \
    allinfo/widgetallinfo.ui \
    switch/widgetswitch.ui \
    switch/widgetcode.ui \
    tasklist/widgetaddtask.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    image.qrc

DEFINES += BOOST_LOG_DYN_LINK

unix:!macx|win32: LIBS += -L$$PWD/third_party/lib/ -lLogger

INCLUDEPATH += $$PWD/third_party/include
DEPENDPATH += $$PWD/third_party/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/third_party/lib/Logger.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/third_party/lib/libLogger.a
