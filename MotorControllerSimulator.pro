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
        MainWindow.cpp \
    Communication/Communication.cpp \
    Communication/CommunicationSerialPort.cpp \
    Communication/CommunicationTcpSocket.cpp \
    Communication/CommunicationTcpSocketClient.cpp \
    Communication/CommunicationTcpSocketServer.cpp \
    Application.cpp \
    DataParser.cpp \
    Simulator/DCMotor.cpp \
    Simulator/PICtrler.cpp \
    Simulator/RaceCar.cpp \
    Simulator/Accumulator.cpp

HEADERS  += MainWindow.h \
    Communication/Communication.h \
    Communication/CommunicationSerialPort.h \
    Communication/CommunicationTcpSocket.h \
    Communication/CommunicationTcpSocketClient.h \
    Communication/CommunicationTcpSocketServer.h \
    Application.h \
    DataParser.h \
    Simulator/DCMotor.h \
    Simulator/PICtrler.h \
    Simulator/RaceCar.h \
    Simulator/Accumulator.h

FORMS    += MainWindow.ui
