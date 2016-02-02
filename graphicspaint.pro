#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T11:39:57
#
#-------------------------------------------------

QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET += graphicspaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintarea.cpp \
    fdbscene.cpp \
    logicitem.cpp \
    primitive.cpp \
    lineitem.cpp \
    initcableline.cpp

HEADERS  += mainwindow.h \
    paintarea.h \
    fdbscene.h \
    logicitem.h \
    primitive.h \
    lineitem.h \
    initcableline.h

RESOURCES += \
    graphicspaint.qrc


