#ifndef RACECAR_H
#define RACECAR_H

#include "DCMotor.h"
#include "PICtrler.h"
#include "Accumulator.h"
#include <QObject>
#include <QTimer>
#include <QDebug>

enum raceCarState{
    stop, hven, dren
};

class RaceCar : public QObject
{
    Q_OBJECT
public:
    RaceCar(QObject *parent = 0, double dtm=1,
            double dtpi=5, double D=0.2,
            double C=10, double m=200);
    double GetSpeed(){return vpre;}
    double GetAngSpeed(){return motor.GetW();}
    double GetTorq(){return (motor.GetM()+C*vpre*vpre);}
    double GetCurr(){return motor.GetI();}
    double GetCapac(){return accu.GetAh();}
    double GetVbat(){return accu.GetVbat();}
    double GetVrail(){return accu.GetVrail();}
    double GetVref(){return vref;}

private:
    QTimer motTmr, PITmr;
    DCMotor motor;
    PICtrler ctrler;
    Accumulator accu;
    raceCarState state;

    double dtm, dtpi, D, C, m, vpre;
    double Mref, vref;

    void UpdateState(double M, double v);

    void PrintDataToDebug();

signals:
    void MotorTick(double Mt, double u, double dt);
    void PITick(double uBase, double uSense, double Ts, double umax);
    void AccuUpdate(double i, double dt);
    void sendString(const QString&);
public slots:
    void MotTimeout();
    void PITimeout();
    void DataProc(QMap<quint16, double>&, QMap<QString, quint16>&);
};

#endif // RACECAR_H
