#-------------------------------------------------
#
# Project created by QtCreator 2015-11-20T11:38:33
#
#-------------------------------------------------
CONFIG += c++14
QMAKE_CXXFLAGS_CXX11    = -std=c++1y
QT += core gui
QT += qml quick widgets
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotorControllerSimulator
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
