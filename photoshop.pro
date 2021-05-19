QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Photoshop_opencv
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += opencv

INCLUDEPATH += C:\opencv\build\include
        C:\opencv\build\include\opencv2


CONFIG(debug,debug|release){

    LIBS += -LC:\opencv\build\x64\vc15\lib -lopencv_world451d

}
CONFIG(release,debug|release){

    LIBS += -LC:\opencv\build\x64\vc15\lib -lopencv_world451
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        photo_frame.cpp

HEADERS += \
        mainwindow.h \
        photo_frame.h \
        stdafx.h

FORMS += \
        mainwindow.ui \
        photo_frame.ui
