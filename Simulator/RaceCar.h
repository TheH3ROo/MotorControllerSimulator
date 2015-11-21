#ifndef RACECAR_H
#define RACECAR_H

#include "DCMotor.h"
#include "PICtrler.h"
#include <QObject>
#include <QTimer>
#include <QDebug>

class RaceCar : public QObject
{
    Q_OBJECT
public:
    RaceCar(QObject *parent = 0, double dtm=1,
            double dtpi=20, double D=0.2,
            double C=1, double m=200);
private:
    QTimer mainTmr, PITmr;
    DCMotor motor;
    PICtrler ctrler;

    double dtm, dtpi, D, C, m, vpre;
    double Mbase, wbase;
signals:
    void MotorTick(double Mt, double u, double dt);
    void PITick(double uBase, double uSense, double Ts);
public slots:
    void MotTimeout();
    void PITimeout();
};

#endif // RACECAR_H
