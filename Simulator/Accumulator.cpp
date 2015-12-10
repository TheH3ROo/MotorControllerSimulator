#include "Accumulator.h"
#include <QDebug>

const double umax_def = 50;

const double umin_def = 36;

const double ah_def = 10;
const double ah_min = 0;
const quint8 cellnum_def = 12;

Accumulator::Accumulator():
    ucell(cellnum_def, 0)
{
    umax=umax_def;
    umin=umin_def;
    ah=ah_def;
    u=umax;
    cellnum = cellnum_def;

    celldiff.append(1.01);
    celldiff.append(0.98);
    celldiff.append(0.97);
    celldiff.append(1.02);
    celldiff.append(1.01);
    celldiff.append(1);
    celldiff.append(1.03);
    celldiff.append(0.95);
    celldiff.append(1.015);
    celldiff.append(1.022);
    celldiff.append(1.);
    celldiff.append(0.993);

    UpdateCells();
}

bool Accumulator::isOk()
{
    if(ah > 0)
        return true;
    else
        return false;
}

void Accumulator::UpdateCells()
{
    quint8 i;
    double vcell = u / cellnum;
    for(i=0; i < cellnum_def; ++i)
    {
        ucell[i] = vcell * celldiff[i];
    }
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
            u=-(umax-umin)/ah_def*(ah_def-ah) + umax;
            urail = u;
        }
        UpdateCells();
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
