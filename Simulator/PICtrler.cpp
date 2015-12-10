#include "PICtrler.h"

PICtrler::PICtrler(QObject *parent,
                   double Ap,
                   double Ti)
    : QObject(parent), Ap(Ap), Ti(Ti)
{
    integral = 0;
    out = 0;
    xpre = 0;
}

void PICtrler::Tick(double uBase, double uSense, double Ts, double umax)
{
    double x = (uBase - uSense)*Ap;
    integral = integral + x*Ts/Ti;
    out = integral + x;
    if (out < -umax/2)
    {
        integral = -umax/2;
        out = -umax/2;
    }
    if (out > umax/2)
    {
        integral = umax/2;
        out = umax/2;
    }
    xpre = x;
}
