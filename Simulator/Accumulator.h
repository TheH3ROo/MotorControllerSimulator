#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
#include <QObject>
#include <QVector>

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
    const QVector<double>& GetVCell(){return ucell;}
    bool HVEN(bool);
    bool isOk();

private:
    double umax, umin, ah, u, urail, cellnum;
    QVector<double> ucell, celldiff;
    contactorState contactor;

    void UpdateCells();

public slots:
    void UpdateCapacity(double i, double dt);
};

#endif // ACCUMULATOR_H
