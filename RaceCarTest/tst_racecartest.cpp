#include <QString>
#include <QtTest>
#include <QMap>
#include "Simulator/RaceCar.h"
#include "DataParser.h"

class RaceCarTest : public QObject
{
    Q_OBJECT

public:
    RaceCarTest();

signals:
    void dataToCar(QMap<quint16, double>& data, QMap<QString, quint16>& code);

private Q_SLOTS:
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();

private:
    RaceCar car;
    DataParser dataParser;
    QMap<quint16, double> testData;
    QMap<QString, quint16> testCode;
};

RaceCarTest::RaceCarTest() : car(), dataParser("code.txt")
{
    testCode = dataParser.getCodeMap();
    testData = dataParser.getDataMap();
    QObject::connect(this, SIGNAL(dataToCar(QMap<quint16,double>&,QMap<QString,quint16>&)),
                     &car, SLOT(DataProc(QMap<quint16,double>&,QMap<QString,quint16>&)));
}

void RaceCarTest::testCase1()
{
    /** Ez stop állapotot jelent, az autónak ilyen állapotban kell lennie.*/
    testData[testCode["state"]] = 1;
    emit dataToCar(testData, testCode);
    QVERIFY2(car.IsStop() > 1, "Az autó nincs álló helyzetben!");
    QVERIFY2(car.IsHVEN() < 1, "Az autó nincs álló helyzetben!");
    QVERIFY2(car.IsDREN() < 1, "Az autó nincs álló helyzetben!");
}

void RaceCarTest::testCase2()
{
    /** Az autóban a DC sín feszültségnek 0-nak kell lenni, ha nincs HVEN állapotban.*/
    testData[testCode["state"]] = 1;
    emit dataToCar(testData, testCode);
    QVERIFY2(car.IsStop() > 1, "Az autó nincs álló állapotban!");
    QVERIFY2(car.GetVrail() < 1e-3, "Az autó DC sín feszültség alatt van!");
}
void RaceCarTest::testCase3()
{
    /** Ez hven állapotot jelent, az autónak ilyen állapotban kell lennie.*/
    testData[testCode["state"]] = 10;
    emit dataToCar(testData, testCode);
    QVERIFY2(car.IsStop() < 1, "Az autó nincs HVEN állapotban!");
    QVERIFY2(car.IsHVEN() > 1, "Az autó nincs HVEN állapotban!");
    QVERIFY2(car.IsDREN() < 1, "Az autó nincs HVEN állapotban!");
}

void RaceCarTest::testCase4()
{
    /** Ez dren állapotot jelent, az autónak ilyen állapotban kell lennie.*/
    testData[testCode["state"]] = 20;
    emit dataToCar(testData, testCode);
    QVERIFY2(car.IsStop() < 1, "Az autó nincs DREN állapotban!");
    QVERIFY2(car.IsHVEN() < 1, "Az autó nincs DREN állapotban!");
    QVERIFY2(car.IsDREN() > 1, "Az autó nincs DREN állapotban!");
}

void RaceCarTest::testCase5()
{
    /** Az autó megkapja a sebesség referenciát és dren állapotba kerül.*/
    testData[testCode["state"]] = 20;
    testData[testCode["vref"]] = 24.5;
    emit dataToCar(testData, testCode);
    QVERIFY2(car.IsStop() < 1, "Az autó nincs DREN állapotban!");
    QVERIFY2(car.IsHVEN() < 1, "Az autó nincs DREN állapotban!");
    QVERIFY2(car.IsDREN() > 1, "Az autó nincs DREN állapotban!");
    QVERIFY2(car.GetVref() == 24.5 , "Az autó nem kapta meg a sebességreferenciát!");
}

QTEST_APPLESS_MAIN(RaceCarTest)

#include "tst_racecartest.moc"
