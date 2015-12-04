#pragma once
#ifndef STVAPPLICATION_H
#define STVAPPLICATION_H
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include "DataParser.h"
#include "mainwindow.h"
#include "Communication/CommunicationTcpSocketServer.h"
#include "Communication/CommunicationTcpSocketClient.h"
#include "Communication/CommunicationSerialPort.h"
#include "Simulator/RaceCar.h"



/**
 * @brief Alkalmazás osztály. A main() példányosítja és indítja el.
 *
 * A konstruktor elvégzi az alapvető példányosításokat és a signalok összekapcsolását.
 */
class Application : public QApplication
{
    Q_OBJECT

public:
    /** Konstruktor. Alapvető példányosítások és signal bekötések. */
    Application(int argc, char *argv[]);
    //~Application() = default;
private:
    RaceCar car;
    QQmlApplicationEngine engine;
    CommunicationTcpSocketServer tcpServer;
    CommunicationTcpSocketClient tcpClient;
    DataParser dataParser;
    CommunicationSerialPort serialPort;
    QTimer dataSendTimer;

    void SendData(quint16 code, double value);
    void SendDataFromClient(quint16 code, double value);
    void PutInByteArray(quint16 code, double value, QByteArray& ba);

signals:
    //void dataReady(QMap<quint16, double>&, QMap<QString, quint16>&);

public slots:
    void errorHandling(const QString&);
    void sendString(const QString&);

private slots:
    void sendData();
    void sendDataDebug();
    void startDataSendTimer(){dataSendTimer.start();}
    void stopDataSendTimer(){dataSendTimer.stop();}
};

#endif // STVAPPLICATION_H
