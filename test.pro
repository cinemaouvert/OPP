TARGET = unittest
TEMPLATE = app

QT += testlib
QT -= gui

CONFIG += console
CONFIG -= app_bundle

DEPENDPATH += ./src
INCLUDEPATH += ./src

SOURCES += test/main.cpp \
    test/test1.cpp \
    test/test2.cpp

HEADERS += test/autotest.h \
    test/test1.h \
    test/test2.h
