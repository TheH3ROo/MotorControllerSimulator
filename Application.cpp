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
    connect(&tcpServer,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    connect(&tcpClient,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    connect(&serialPort,SIGNAL(dataReady(QDataStream&)),
            &dataParser,SLOT(dataInput(QDataStream&)));
    /** A dataParser és a car közötti jelek összekötése.*/
    connect(&dataParser,SIGNAL(dataReady(QMap<quint16, double>&, QMap<QString, quint16>&)),
            &car,SLOT(DataProc(QMap<quint16, double>&, QMap<QString, quint16>&)));
    /** A hibajelek signaljainak összekötése.*/
    connect(&tcpServer,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    connect(&tcpServer,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    connect(&dataParser,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));
    connect(&serialPort,SIGNAL(errorOccurred(const QString&)),
            this,SLOT(errorHandling(const QString&)));

//DEBUG!
    /** Kommunikáció indítása, kapcsolódás.*/
    tcpClient.connect(QString("localhost"),4444);
    serialPort.connect();
//DEBUG! vége

    /** Adat frissítés, timer indítása.*/
    dataUpdateTimer = new QTimer(this);
    dataUpdateTimer->stop();
    connect(dataUpdateTimer,SIGNAL(timeout()),
            &dataParser,SLOT(saveDataTimestamp()));
    /** Folyamatos timer legyen.*/
    dataUpdateTimer->setSingleShot(false);
    dataUpdateTimer->setInterval(500);
    dataUpdateTimer->start();
}

void Application::errorHandling(const QString& error)
{
    qWarning() << "Hiba!";
    qWarning() << error << endl;
}
