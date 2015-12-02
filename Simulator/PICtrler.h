#ifndef PICTRLER_H
#define PICTRLER_H

#include <QObject>

class PICtrler : public QObject
{
    Q_OBJECT
public:
    explicit PICtrler(QObject *parent = 0,
                      double Ap = 10,
                      double Ti = 1e-3);
    double GetValue(){return out;}

private:
    const double Ap, Ti;
    double integral, xpre, out;

signals:

public slots:
    void Tick(double uBase, double uSense, double Ts, double umax);
};

#endif // PICTRLER_H