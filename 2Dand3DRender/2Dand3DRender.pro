#Made By Jiangcaiyang at 2013年1月12日

HEADERS += \
    GLWidget.h

SOURCES += \
    main.cpp \
    GLWidget2.cpp \
    GLWidget.cpp
TRANSLATIONS += zh_CN.ts
QT += opengl
RESOURCES += Resource.qrc

OTHER_FILES += zh_CN.ts

TARGET = 2Dand3DRender
unix {# 适用于Unix/Linux的
    LIBS += -lGLU
}
win32 {# 适用于Windows的
    DEFINES += _WIN32
    LIBS += -lglu32
}
