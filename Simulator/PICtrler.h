#ifndef PICTRLER_H
#define PICTRLER_H

#include <QObject>

class PICtrler : public QObject
{
    Q_OBJECT
public:
    explicit PICtrler(QObject *parent = 0,
                      double Ap = 1,
                      double Ti = 10,
                      double umax = 330);
    double GetValue(){return out;}

private:
    const double Ap, Ti, umax;
    double integral, xpre, out;

signals:

public slots:
    void Tick(double uBase, double uSense, double Ts);
};

#endif // PICTRLER_H
