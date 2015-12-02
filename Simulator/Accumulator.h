#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
#include <QObject>

class Accumulator: public QObject
{
    Q_OBJECT

public:
    Accumulator();
    double GetVoltage(){return u;}
    double GetAh(){return ah;}

private:
    double umax, umin, ah, u;

public slots:
    void UpdateCapacity(double i, double dt);
};

#endif // ACCUMULATOR_H
