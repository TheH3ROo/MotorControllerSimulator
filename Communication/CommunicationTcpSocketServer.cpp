#include "CommunicationTcpSocketServer.h"

CommunicationTcpSocketServer::CommunicationTcpSocketServer(int port)
    : serverSocket(this)
{
    if (!serverSocket.listen(QHostAddress::Any, port))
    {
        qWarning() << "Nem sikerült megnyitni a szerver socketet: ";
        qWarning() << serverSocket.errorString();
    }
    else
    {
        qWarning() << "A szerver socket kapcsolatra vár.";
    }

    connect(&serverSocket, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void CommunicationTcpSocketServer::newConnection()
{
    QTcpSocket *newSocket = serverSocket.nextPendingConnection();
    if (newSocket)
    {
        if (newSocket->isOpen()) {
            connect(newSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        }
        setSocket(newSocket);
        qWarning() << "Új kapcsolat létesült.\n";


        QByteArray ba;
        ba[0]=0x00;
        ba[1]=0x01;
        ba[2]=100;
        send(ba);

        ba[0]=0x00;
        ba[1]=0x02;
        ba[2]=0;
        send(ba);

        ba[0]=0x00;
        ba[1]=0x04;
        ba[3]=192;
        send(ba);

    }
}

void CommunicationTcpSocketServer::disconnected()
{
    qWarning() << "A szerver oldali socket lezáródott.";
}
