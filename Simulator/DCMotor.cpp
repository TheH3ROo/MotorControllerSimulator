#include "DCMotor.h"

DCMotor::DCMotor(QObject *parent,
                 double kf, double R,
                 double L, double theta):
    kf(kf), R(R), L(L), theta(theta)
{
    i = 0;
    ipre = 0;
    w = 0;
    wpre = 0;
    M = 0;
}

void DCMotor::Tick(double Mt, double u, double dt)
{
    dt = dt;
    i = (u-kf*wpre+L*ipre/dt)/(R+L/dt+kf*kf*dt/theta);
    M = kf*i - Mt;
    w = M*dt/theta+wpre;

    ipre = i;
    wpre = w;
}

