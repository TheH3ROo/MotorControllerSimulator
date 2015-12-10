#pragma once
#ifndef STVAPPLICATION_H
#define STVAPPLICATION_H
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVector>
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
 * Tartalmazza a kommunikációs obejektumokat, adatparsolást, biztosítja az objektumok közötti kommunikációt.
 */
class Application : public QApplication
{
    Q_OBJECT

public:
    /** Konstruktor. Alapvető példányosítások és signal bekötések. */
    Application(int argc, char *argv[]);

private:
    /** Obejktumok példányosítása.*/

    RaceCar car;
    QQmlApplicationEngine engine;
    CommunicationTcpSocketServer tcpServer;
    CommunicationTcpSocketClient tcpClient;
    DataParser dataParser;
    CommunicationSerialPort serialPort;
    QTimer dataSendTimer;

    /** Egy adatot küld el a TCP szerverről a kliensnek. Paraméterek: @see DataParser*/
    void SendData(quint16, double);
    /** Egy adatot küld el a TCP kliensről a szervernek. Paraméterek: @see DataParser*/
    void SendDataFromClient(quint16, double);
    /** Ahhoz, hogy egyszerre tudjuk elküldeni az összes adatot a kliens számára,
     * minden adatot egy ByteArray-be kell elhelyezni, ezt tudjuk átadni a kommunikációs
     * objektumoknak(@see Communication::send). Ez a függvény ezt a folyamatot segíti meg,
     * elrejti a felhasználó elől az elsőre bonyolultnak tűnő műveleteket.
     * @param code  Adathoz tartozó 16bites kód. @see DataParser
     * @param value Az elküldendő adat.
     * @param ba    Ebbe a ByteArray-be kerül bele az adat.
    */
    void PutInByteArray(quint16 code, double value, QByteArray& ba);
    /** A cellafeszültségeket vektorként küldjük el, ez a függvény ebben nyújt segítséget.
     * @param code  A cellafeszültségekhez tartozó kód. @see DataParser
     * @param value Az elküldendő adatvektor.
     * @param ba    Ebbe a ByteArray-be kerül bele az adat.
    */
    void PutVectorInByteArray(quint16 code, QVector<double>& value, QByteArray& ba);

public slots:
    /** Hibakezelésért felelős slot.*/
    void errorHandling(const QString&);
    /** A paraméterként kapott stringet elküldi a kliensnek.*/
    void sendString(const QString&);

private slots:
    /** Hatására az Application::car objektumtól lekért adatokat elküldi a TCP kliensnek.*/
    void sendData();
    /** Hibakereséshez használható változat. @see Application:sendData*/
    void sendDataDebug();
    void startDataSendTimer(){dataSendTimer.start();}
    void stopDataSendTimer(){dataSendTimer.stop();}
};

#endif // STVAPPLICATION_H
