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
    if(byteArray.size() >= 2+sizeof(double))
    {
        quint16 code;
        double value;
        memcpy(&code, byteArray.data(), 2);
        memcpy(&value, byteArray.data()+2, sizeof(double));
        dataMap[code]=value;
    }

//DEBUG!
    PrintDataToDebug();
//DEBUG!

    emit dataReady(dataMap, codeMap);

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

