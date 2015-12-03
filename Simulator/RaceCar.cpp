#include "RaceCar.h"

RaceCar::RaceCar(QObject *parent, double dtm, double dtpi, double D, double C, double m)
    : QObject(parent), dtm(dtm), dtpi(dtpi), D(D), C(C), m(m)
{
    vpre = 0;
    vref = 0;
    Mref = 0;

    state = stop;

    motTmr.setInterval(dtm);
    PITmr.setInterval(dtpi);

    connect(&motTmr, &QTimer::timeout,
            this, &RaceCar::MotTimeout);
    connect(&PITmr, &QTimer::timeout,
            this, &RaceCar::PITimeout);
    connect(this, &RaceCar::MotorTick,
            &motor, &DCMotor::Tick);
    connect(this, &RaceCar::PITick,
            &ctrler, &PICtrler::Tick);
    connect(this, &RaceCar::AccuUpdate,
            &accu, &Accumulator::UpdateCapacity);
    motTmr.start();
    PITmr.start();
}

void RaceCar::MotTimeout()
{
    double w = motor.GetW();
    double v = w/D*2;
    double Fs = C*v*v;
    double Mt = D/2*(Fs);
    if(state == dren)
        emit MotorTick(Mt, ctrler.GetValue(), dtm/1000);
    else
        emit MotorTick(Mt, 0, dtm/1000);

    emit AccuUpdate(motor.GetI(),dtm/1000);
    vpre = v;
//DEBUG!
    //PrintDataToDebug();
}

void RaceCar::PITimeout()
{
    emit PITick(vref*D/2, motor.GetW(), dtpi/1000, accu.GetVrail());
}

void RaceCar::UpdateState(double M, double v)
{
    switch(state)
    {
        case stop:
            Mref = vref = 0;
            accu.HVEN(false);
            break;
        case hven:
            if(!accu.HVEN(true))
            {
                state = stop;
                break;
            }
            Mref = vref = 0;
            break;
        case dren:
            Mref = M;
            vref = v;
            break;
        default:
            break;
    }
}

void RaceCar::DataProc(QMap<quint16, double>& data, QMap<QString, quint16>& code)
{
    //State beállítása
    if(data.value(code["hven"]) > 1)
    {
        if(data.value(code["dren"]) > 1)
            state=dren;
        else
            state=hven;
    }
    else
        state=stop;
    //Alapjelek beállítása
    UpdateState(data.value(code["mref"]), data.value(code["vref"]));
}

void RaceCar::PrintDataToDebug()
{
    qDebug() << "--------------------------------";
    switch(state)
    {
        case stop:
            qDebug() << "STOP";
            break;
        case hven:
            qDebug() << "HVEN";
            break;
        case dren:
            qDebug() << "DREN";
            break;
        default:
            qDebug() << "DEFAULT";
            break;
    }
    qDebug() << "vref="<<vref<<"m/s";
    qDebug() << "Mref="<<Mref<<"Nm";
    qDebug() << "v="<<vpre<<"m/s";
    qDebug() << "w="<<motor.GetW()<<"rad/s";
    qDebug() << "M="<<motor.GetM()<<"Nm";
    qDebug() << "I="<<motor.GetI()<<"A";
    qDebug() << "C="<<accu.GetAh()<<"Ah";
    qDebug() << "Vbat="<<accu.GetVbat()<<"V";
    qDebug() << "Vrail="<<accu.GetVrail()<<"V";
}
