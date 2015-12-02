#include "Accumulator.h"
#include <QDebug>

Accumulator::Accumulator()
{
    umax=50;
    umin=40;
    ah=40;
    u=umax;
}

void Accumulator::UpdateCapacity(double i, double dt)
{
    ah = ah - i*dt/3600;
    if(ah<=0)
    {
        ah=0;
        u=0;
    }
    else
    {
        u=(umax-umin)/ah + umin;
    }
}

