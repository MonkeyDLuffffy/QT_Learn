#-------------------------------------------------
#
# Project created by QtCreator 2022-07-28T22:21:04
#
#-------------------------------------------------
CONFIG +=C++11
QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LearnOpengl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ./opengl
INCLUDEPATH += ./opengl/model
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    opengl/myopenglwidget.cpp \
    opengl/actor.cpp \
    opengl/scene.cpp \
    opengl/camera.cpp \
    environmentsettingdialog.cpp \
    opengl/model/mesh.cpp \
    opengl/model/model.cpp \
    opengl/model/light.cpp \
    opengl/model/lightbase.cpp

HEADERS += \
        mainwindow.h \
    opengl/myopenglwidget.h \
    opengl/actor.h \
    opengl/scene.h \
    opengl/data.h \
    opengl/camera.h \
    environmentsettingdialog.h \
    opengl/model/mesh.h \
    opengl/model/model.h \
    opengl/model/light.h \
    opengl/model/lightbase.h

FORMS += \
        mainwindow.ui \
    environmentsettingdialog.ui

RESOURCES += \
    images/image.qrc \
    shader/shaders.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lassimp-vc142-mt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lassimp-vc142-mtd

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
