#include "Application.h"

/** A szerver hallgatózás az inicializáláskor megkezdődik.
 *
 */
Application::Application(int argc, char *argv[])
    : QApplication(argc, argv), engine(),
      tcpServer(4444), tcpClient(),
      dataParser(QString("code.txt")),
      serialPort(QString("COM4"),QSerialPort::Baud9600,QSerialPort::Data8,QSerialPort::NoParity,QSerialPort::OneStop)
{    
    /** A dataParser és a kommunikációs objektumok signaljainak összekötése.*/
    QObject::connect(&tcpServer,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    QObject::connect(&tcpClient,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    QObject::connect(&serialPort,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    /** A dataParser és a car közötti jelek összekötése.*/
    QObject::connect(&dataParser,SIGNAL(dataReady(QMap<quint16, double>&, QMap<QString, quint16>&)),
            &car,SLOT(DataProc(QMap<quint16, double>&, QMap<QString, quint16>&)));
    /** A hibajelek signaljainak összekötése.*/
    QObject::connect(&tcpServer,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    QObject::connect(&tcpServer,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    QObject::connect(&dataParser,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    QObject::connect(&serialPort,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));

//DEBUG!
    /** Kommunikáció indítása, kapcsolódás.*/
    tcpClient.connect(QString("152.66.180.1"),4444);
    serialPort.connect();
//DEBUG! vége

    /** Adat küldés timer inicializálása.*/
    connect(&dataSendTimer,SIGNAL(timeout()),
            this,SLOT(sendData()));
    /** Folyamatos timer legyen.*/
    dataSendTimer.setSingleShot(false);
    dataSendTimer.setInterval(5000);
    dataSendTimer.start();
    /*
    sendData();
    tcpClient.SendDebugData(0x01, 10);
    tcpClient.SendDebugData(0x02, 10);
    tcpClient.SendDebugData(0x4, 12.6);*/
}

void Application::SendData(quint16 code, double value)
{
    QByteArray ba(reinterpret_cast<const char*>(&code), sizeof(quint16));
    QByteArray byteArray(reinterpret_cast<const char*>(&value), sizeof(double));
    ba.insert(2,byteArray);
    tcpServer.send(ba);
}

void Application::SendDataFromClient(quint16 code, double value)
{
    QByteArray ba(reinterpret_cast<const char*>(&code), sizeof(quint16));
    QByteArray byteArray(reinterpret_cast<const char*>(&value), sizeof(double));
    ba.insert(2,byteArray);
    tcpClient.send(ba);
}

void Application::sendData()
{
    if(tcpClient.state() == QAbstractSocket::ConnectedState)
    {
        SendData(dataParser.GetCode(QString("speed")),car.GetSpeed());
        SendData(dataParser.GetCode(QString("angspeed")),car.GetAngSpeed());
        SendData(dataParser.GetCode(QString("torq")),car.GetTorq());
        SendData(dataParser.GetCode(QString("curr")),car.GetCurr());
        SendData(dataParser.GetCode(QString("capac")),car.GetCapac());
        SendData(dataParser.GetCode(QString("vacc")),car.GetVbat());
        SendData(dataParser.GetCode(QString("vrail")),car.GetVrail());

        SendDataFromClient(dataParser.GetCode(QString("hven")),0x10);
        SendDataFromClient(dataParser.GetCode(QString("dren")),0x10);
        SendDataFromClient(dataParser.GetCode(QString("vref")),5);
        qDebug() << "Adatok elküldve.";
    }
    else
        qDebug() << "Adatok nincsenek elküldve. Socket állapot: "<<tcpClient.state();
}

void Application::errorHandling(const QString& error)
{
    qWarning() << "Hiba!";
    qWarning() << error << endl;
}
