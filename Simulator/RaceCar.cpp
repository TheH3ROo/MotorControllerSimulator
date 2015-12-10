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

double RaceCar::IsStop()
{
    if(state == stop)
        return 10;
    else
        return 0;
}

double RaceCar::IsDREN()
{
    if(state == dren)
        return 10;
    else
        return 0;
}

double RaceCar::IsHVEN()
{
    if(state == hven)
        return 10;
    else
        return 0;
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
    QString str("State: ");
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
            if(!accu.HVEN(true))
            {
                state = stop;
                break;
            }
            Mref = M;
            vref = v;
            break;
        default:
            break;
    }
}

void RaceCar::SelfTest()
{
    if(accu.isOk())
    {
        emit sendString("SelfTest OK: Akku rendben.");
    }
    else
    {
        emit sendString("SelfTest FAULT: Akku lemerült.");
    }
    if(ctrler.isOk())
    {
        emit sendString("SelfTest OK: PI szabályzó rendben.");
    }
    else
    {
        emit sendString("SelfTest FAULT: PI szabályozó hibás.");
    }
    if(motor.isOk())
    {
        emit sendString("SelfTest OK: DC motor rendben.");
    }
    else
    {
        emit sendString("SelfTest FAULT: DC motor hibás.");
    }
}

void RaceCar::DataProc(QMap<quint16, double>& data, QMap<QString, quint16>& code)
{
    //State beállítása
    QString str;
    if(data.value(code["state"]) < 5)
    {
        if(state!=stop)
            emit sendString("Az autó megáll.");
        state=stop;
    }
    else if(data.value(code["state"]) > 5)
    {
        if(data.value(code["state"]) > 15)
        {
            if(state!=dren)
                emit sendString("Hajtás engedélyezve. Indulás!");
            state=dren;
        }
        else
        {
            if(state!=hven)
                emit sendString("DC sín feszültség alá helyezve!");
            state=hven;
        }
    }
    else
    {
        state=stop;
    }
    //Alapjelek beállítása
    UpdateState(data.value(code["mref"]), data.value(code["vref"]));
    if(data.value(code["selftest"]) > 1)
    {
        SelfTest();
        data[code["selftest"]] = 0;
        qDebug()<<"selftest.";
    }
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
