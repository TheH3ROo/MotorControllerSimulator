#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
#include <QObject>

class Accumulator: public QObject
{
    Q_OBJECT

enum contactorState
{
    closed, opened
};

public:
    Accumulator();
    double GetVbat(){return u;}
    double GetVrail(){return urail;}
    double GetAh(){return ah;}
    bool HVEN(bool);

private:
    double umax, umin, ah, u, urail;
    contactorState contactor;

public slots:
    void UpdateCapacity(double i, double dt);
};

#endif // ACCUMULATOR_H
