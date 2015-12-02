#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include <QErrorMessage>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>
#include <QDateTime>
#include <QQueue>
#include <QtGlobal>

class DataParser : public QObject
{
    Q_OBJECT

public:
    DataParser(const QString&);
//DEBUG!
    void PrintDataToDebug();
//DEBUG!
    /** Visszatér az adatsorra mutató pointerrel.*/
    QQueue<QMap<quint16, double>>* GetData(){return &dataQueue;}
    /** Segédfüggvény az adatstring-kód kereséséhez.*/
    quint16 GetCode(QString& str){return codeMap[str];}

private:
    /** A kód-jelentést tartalmazó txt fájl objektum.*/
    QFile codeFile;
    /** A kód-jelentést tartalmazó asszociatív tároló.*/
    QMap<QString, quint16> codeMap;
    /** Adatokat tartalmazó map, két időegység között folyamatosan töltődik fel adattal.*/
    QMap<quint16, double> dataMap;

    /** Adat tárolása.*/
    QQueue<QMap<quint16, double>> dataQueue;
signals:
    void errorOccurred(const QString&);
    void dataReady(QMap<quint16, double>&, QMap<QString, quint16>&);

public slots:
    /** Ide lehet bekötni a kommunikációs dataReady signalokat.*/
    void dataInput(QDataStream&);

};

