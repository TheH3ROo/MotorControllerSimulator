#include "Accumulator.h"
#include <QDebug>

const double umax_def = 50;
const double umin_def = 40;
const double ah_def = 40;
const double ah_min = 0;

Accumulator::Accumulator()
{
    umax=umax_def;
    umin=umin_def;
    ah=ah_def;
    u=umax;
}

void Accumulator::UpdateCapacity(double i, double dt)
{
    if(contactor == closed)
    {
        ah = ah - i*dt/3600;
        if(ah <= ah_min)
        {
            ah = ah_min;
            urail = 0;
            contactor = opened;
        }
        else
        {
            u=-(umax-umin)/ah*(ah_def-ah) + umax;
        }
    }
}

bool Accumulator::HVEN(bool en)
{
    if(en)
    {
        if(ah <= ah_min)
        {
            ah = ah_min;
            urail = 0;
            contactor = opened;
            return !en;
        }
        else
        {
            urail = u;
            contactor = closed;
            return en;
        }
    }
    else
    {
        urail = 0;
        contactor = opened;
        return en;
    }
}
