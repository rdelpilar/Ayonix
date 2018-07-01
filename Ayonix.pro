#-------------------------------------------------
#
# Project created by QtCreator 2018-06-25T18:55:33
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ayonix
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    controller.cpp \
    devicemanager.cpp \
    streamworker.cpp \
    cglwidget.cpp

HEADERS += \
        mainwindow.h \
    singleton.h \
    controller.h \
    devicemanager.h \
    streamworker.h \
    cglwidget.h \
    AyonixFaceID.h \
    AyonixTypes.h

FORMS += \
        mainwindow.ui

#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS

#LIBS += -LC:\ffmpeg-20180619-a990184-win32-dev\lib
LIBS += -LC:\ffmpeg-20180619-a990184-win64-dev\lib
LIBS += -lavcodec -lavformat -lavutil -lswscale -lavdevice -lOpengl32
LIBS += -LC:\Users\Ranel\Documents\QtProjects\Ayonix\lib -lAyonixFaceID

#INCLUDEPATH += C:\ffmpeg-20180619-a990184-win32-dev\include
INCLUDEPATH += C:\ffmpeg-20180619-a990184-win64-dev\include
