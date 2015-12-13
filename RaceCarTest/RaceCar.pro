#-------------------------------------------------
#
# Project created by QtCreator 2015-12-13T20:20:11
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_racecartest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_racecartest.cpp \
    Simulator/Accumulator.cpp \
    Simulator/DCMotor.cpp \
    Simulator/PICtrler.cpp \
    Simulator/RaceCar.cpp \
    DataParser.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    Simulator/Accumulator.h \
    Simulator/DCMotor.h \
    Simulator/PICtrler.h \
    Simulator/RaceCar.h \
    DataParser.h
