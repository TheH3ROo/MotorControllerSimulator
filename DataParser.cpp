#include "DataParser.h"


DataParser::DataParser (const QString& codeFilePath)
    :codeFile(codeFilePath)
{
    /** Hibát jelez, ha nem sikerült a fájlt megnyitni. */
    if(!codeFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit errorOccurred(QString("Can't open codeMap file."));
        qWarning() << "HIBA a fájl nem található." << endl;
        return;
    }
    QTextStream fileInput(&codeFile);
    QStringList list;
    QString line;
    bool ok;

    /** Feltölti a codeMap változót a fájlban található értékekkel. */
    line = fileInput.readLine();
    while(!line.isNull())
    {
        list = line.split("-");
        codeMap.insert(list.at(1), list.at(0).toInt(&ok, 16));
        line = fileInput.readLine();
    }

    /** A fájlból való betöltés alapján a dataMap inicializációja 0-val.*/
    QMapIterator<QString, quint16> i(codeMap);
    while (i.hasNext())
    {
        i.next();
        dataMap[i.value()]=0;
    }
}

void DataParser::dataInput(QDataStream& stream)
{
    qDebug() << "Adat érkezett"<< endl;

    QByteArray byteArray;
    stream >> byteArray;
    quint16 code = ((quint8)byteArray[0])*256+(quint8)byteArray[1];

    if(code == codeMap.value(QString("temp")))
    {
        quint16 ST = ((quint8)byteArray[2])*256+(quint8)byteArray[3];
        dataMap[code]=175.0*ST/(qPow(2,16)-1)-45;
    }
    else if(code == codeMap.value(QString("hum")))
    {
        quint16 SRH = ((quint8)byteArray[2])*256+(quint8)byteArray[3];
        dataMap[code]=100.0*SRH/(qPow(2,16)-1);
    }
    else if(code == codeMap.value(QString("vbat")))
    {
        quint16 vbat=((quint8)byteArray[2])*256+(quint8)byteArray[3];
        dataMap[code]=vbat;
    }
    PrintDataToDebug();
    return;
}

// Test for master branch

void DataParser::PrintDataToDebug()
{
    QMapIterator<quint16, double> i(dataMap);
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key() << ": " << i.value() << endl;
    }
}

void DataParser::saveDataTimestamp()
{
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    timestampQueue.enqueue(currTime);
    dataQueue.enqueue(dataMap);

    qDebug() << currTime.toString("yyyy.MM.dd. hh:mm:ss:zzz");
    QMapIterator<quint16, double> i(dataMap);
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key() << ": " << i.value() << endl;
    }
}

