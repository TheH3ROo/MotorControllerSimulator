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

/** Ts in s*/
void PICtrler::Tick(double uBase, double uSense, double Ts, double umax)
{
    double x = (uBase - uSense)*Ap;
    integral = integral + x*Ts/Ti;
    out = integral + x;
    if (out < 0)
    {
        integral = 0;
        out = 0;
    }
    if (out > umax)
    {
        integral = umax;
        out = umax;
    }
    xpre = x;
}
