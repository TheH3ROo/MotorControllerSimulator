#include "RaceCar.h"

RaceCar::RaceCar(QObject *parent, double dtm, double dtpi, double D, double C, double m)
    : QObject(parent), dtm(dtm), dtpi(dtpi), D(D), C(C), m(m)
{
    vpre = 0;
    wbase = 10;
    Mbase = 0;

    mainTmr.setInterval(dtm);
    PITmr.setInterval(dtpi);
    connect(&mainTmr, &QTimer::timeout,
            this, &RaceCar::MotTimeout);
    connect(&PITmr, &QTimer::timeout,
            this, &RaceCar::PITimeout);
    connect(this, &RaceCar::MotorTick,
            &motor, &DCMotor::Tick);
    connect(this, &RaceCar::PITick,
            &ctrler, &PICtrler::Tick);
    mainTmr.start();
    PITmr.start();
}

void RaceCar::MotTimeout()
{
    double w = motor.GetW();
    double v = w/D*2;
    double Fs = C*v*v;
    double Mt = D/2*(Fs+m*(v-vpre)/dtm*1000);
    //emit MotorTick(Mt, ctrler.GetValue(), dtm/1000);
    emit MotorTick(Mt, 1, dtm/1000);

    vpre = v;
    qDebug() << w;
}

void RaceCar::PITimeout()
{
    emit PITick(wbase, motor.GetW(), dtpi/1000);
}
