HEADERS += \
    $$PWD/global_helper.h \
    $$PWD/global_config.h \
    $$PWD/tiplabel.h \
    $$PWD/widgetmessagebox.h \
    $$PWD/widgetpage.h

SOURCES += \
    $$PWD/global_helper.cpp \
    $$PWD/global_config.cpp \
    $$PWD/tiplabel.cpp \
    $$PWD/widgetmessagebox.cpp \
    $$PWD/widgetpage.cpp

include(customWidget/customWidget.pri)
