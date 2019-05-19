#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T21:05:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++0x

TARGET = ParrotClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    format.cpp \
    KVP.cpp \
    login.cpp \
    mainwindow.cpp \
    common.cpp \
    manager.cpp

HEADERS  += widget.h \
    format.h \
    KVP.h \
    pdebug.h \
    login.h \
    mainwindow.h \
    common.h \
    manager.h

FORMS    += widget.ui \
    login.ui \
    mainwindow.ui
