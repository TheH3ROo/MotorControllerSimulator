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
    double Mmech;
    i=(u + L/dt*ipre + kf*dt/theta*Mt - kf*wpre) / (L/dt + R + dt/theta*kf*kf);
    Mmech = kf*i - Mt;
    w = (Mmech)*dt/theta+wpre;
    ipre = i;
    wpre = w;
}

