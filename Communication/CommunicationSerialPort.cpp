#include <QDebug>
#include "CommunicationSerialPort.h"

CommunicationSerialPort::CommunicationSerialPort(
    QString portName,
    qint32 baudRate,
    QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity,
    QSerialPort::StopBits stopBits)
    : Communication()
{
    serialPort.setPortName(portName);
    serialPort.setBaudRate(baudRate);
    serialPort.setDataBits(dataBits);
    serialPort.setParity(parity);
    serialPort.setStopBits(stopBits);

    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    QObject::connect(&serialPort, SIGNAL(error(QSerialPort::SerialPortError)),
                     this, SLOT(handleError(QSerialPort::SerialPortError)));
}

void CommunicationSerialPort::connect()
{
    if (!serialPort.open(QIODevice::ReadWrite))
    {
        emit errorOccurred("Cannot open serial port " + serialPort.portName());
        return;
    }

    // A fogadási adatfolyam csatlakoztatása
    // (Ha volt korábbi, az most megszűnik a unique_ptr miatt.)
    receiveStream = std::make_unique<QDataStream>(&serialPort);
    QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    qDebug() << "Soros port nyitva, adatfogadás készen áll.";


    QByteArray ba;
    ba[0]=0x00;
    ba[1]=0x55;
    ba[2]=200;
    ba[3]=222;
    send(ba);

    ba[0]=0x00;
    ba[1]=0xAA;
    ba[2]=228;
    ba[3]=24;
    send(ba);

    ba[0]=0x00;
    ba[1]=0xCB;
    ba[2]=25;
    ba[3]=92;
    send(ba);
}

bool CommunicationSerialPort::isConnected() const
{
    return serialPort.isOpen();
}

void CommunicationSerialPort::sendBufferContent()
{
    if (!isConnected())
    {
        emit errorOccurred(QString("HIBA: Küldésnél nincs nyitva a soros port."));
        return;
    }

    qDebug() << "CommunicationTcpSocket::send() " << sendBuffer.size() << " bájt:\n" << sendBuffer.toHex();
    serialPort.write(sendBuffer);
    sendBuffer.clear();
}

void CommunicationSerialPort::handleError(QSerialPort::SerialPortError error)
{
    Q_UNUSED(error);
    emit this->errorOccurred(serialPort.errorString());
}
