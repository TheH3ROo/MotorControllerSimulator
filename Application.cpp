#include "Application.h"

/** A szerver hallgatózás az inicializáláskor megkezdődik.
 *
 */

const uint byteArrayLen = 42;
const uint dataSendTimerTimeout = 200;

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

    /** Ha a TCP szerverre kapcsolat érkezik, indulhat az adatok kiküldése, előtte felesleges.*/
    QObject::connect(&tcpServer,SIGNAL(newConnectionSignal()),
                     this,SLOT(startDataSendTimer()));
    QObject::connect(&tcpServer,SIGNAL(disconnectedSignal()),
                     this,SLOT(stopDataSendTimer()));

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
    serialPort.connect();
//DEBUG! vége

    /** Adat küldés timer inicializálása.*/
    connect(&dataSendTimer,SIGNAL(timeout()),
            this,SLOT(sendData()));
    /** Folyamatos timer legyen.*/
    dataSendTimer.setSingleShot(false);
    dataSendTimer.setInterval(dataSendTimerTimeout);
}

void Application::PutInByteArray(quint16 code, double value, QByteArray& ba)
{
    uint i = ba.size();
    uint k=(sizeof(quint16) + sizeof(double));
    ba.resize(i+k);
    memcpy(ba.data() + i, &code, sizeof(quint16));
    memcpy(ba.data()+sizeof(quint16)+i, &value, sizeof(double));
    i++;
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
    QByteArray ba;
    PutInByteArray(dataParser.GetCode(QString("speed")), car.GetSpeed(), ba);
    PutInByteArray(dataParser.GetCode(QString("angspeed")), car.GetAngSpeed(), ba);
    PutInByteArray(dataParser.GetCode(QString("torq")), car.GetTorq(), ba);
    PutInByteArray(dataParser.GetCode(QString("curr")), car.GetCurr(), ba);
    PutInByteArray(dataParser.GetCode(QString("capac")), car.GetCapac(), ba);
    PutInByteArray(dataParser.GetCode(QString("vacc")), car.GetVbat(), ba);
    PutInByteArray(dataParser.GetCode(QString("vrail")), car.GetVrail(), ba);
    PutInByteArray(dataParser.GetCode(QString("vref")), car.GetVref(), ba);
    tcpServer.send(ba);
    qDebug() << "Adatok elküldve.";
}

void Application::sendDataDebug()
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
        SendData(dataParser.GetCode(QString("vref")),car.GetVrail());

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

void Application::sendString(const QString & str)
{
    QByteArray ba;
    ba.resize(2);
    quint16 code = dataParser.GetCode("str");
    memcpy(ba.data(),&code,sizeof(quint16));
    ba.append(str);
    tcpServer.send(ba);
}
