#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp01
TEMPLATE = app


SOURCES += main.cpp\
        aquarium.cpp \
        axe.cpp \
        banc.cpp \
        poisson.cpp \
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        aquarium.h \
        axe.h \
        banc.h \
        glarea.h \
        poisson.h

FORMS    += princ.ui

RESOURCES += \
    tp01.qrc
