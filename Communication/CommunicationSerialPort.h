#pragma once
#ifndef COMMUNICATIONSERIALPORT_H
#define COMMUNICATIONSERIALPORT_H
#include "Communication/Communication.h"
#include <QtSerialPort/QSerialPort>

/**
 * @brief Soros port kommunikáció
 */
class CommunicationSerialPort : public Communication
{
public:
    CommunicationSerialPort(QString portName,
            qint32 baudRate,
            QSerialPort::DataBits dataBits,
            QSerialPort::Parity parity,
            QSerialPort::StopBits stopBits);
    virtual ~CommunicationSerialPort() = default;

    virtual void connect();
    virtual bool isConnected() const override;

protected:
    virtual void sendBufferContent() override;

private:
    QSerialPort serialPort;

private slots:
    // Hibakezeléshez.
    void handleError(QSerialPort::SerialPortError error);

};

#endif // COMMUNICATIONSERIALPORT_H
