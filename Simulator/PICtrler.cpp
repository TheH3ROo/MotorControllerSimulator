#include "PICtrler.h"

PICtrler::PICtrler(QObject *parent,
                   double Ap,
                   double Ti,
                   double umax)
    : QObject(parent), Ap(Ap), Ti(Ti), umax(umax)
{
    integral = 0;
    out = 0;
    xpre = 0;
}

void PICtrler::Tick(double uBase, double uSense, double Ts)
{
    double x = (uBase - uSense)*Ap;
    integral = integral + x*Ts/Ti;
    out = integral + x;
    if (out < 0)
    {
        out = 0;
        integral = out - x;
    }
    if (out > umax)
    {
        out = umax;
        integral = out - x;
    }
    xpre = x;
}
