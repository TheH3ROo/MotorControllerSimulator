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

//DEBUG!
void CommunicationTcpSocketServer::SendDebugData(quint16 code, double value)
{
    QByteArray ba(reinterpret_cast<const char*>(&code), sizeof(quint16));
    QByteArray byteArray(reinterpret_cast<const char*>(&value), sizeof(double));
    ba.insert(2,byteArray);
    send(ba);
}

//DEBUG!

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

        SendDebugData(0x01, 10);
        SendDebugData(0x02, 10);
        SendDebugData(0x4, 12.6);
    }
}

void CommunicationTcpSocketServer::disconnected()
{
    qWarning() << "A szerver oldali socket lezáródott.";
}
