#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <QObject>
#include <QDebug>

/** @brief DC motort szimuláló osztály.
 *
 *  A
 */

class DCMotor : public QObject
{
    Q_OBJECT
public:
    DCMotor(QObject *parent = 0,
            double kf=0.1, double R=0.1,
            double L=0.1, double theta=10);
    double GetI(){return i;}
    double GetW(){return w;}
    double GetM(){return kf*i;}
private:
    /** Szimulációval számított értékek.*/
    double i, ipre;
    double w, wpre;
    double M;

    /** DC motor paraméterek.*/
    const double kf;
    const double R,L;
    const double theta;
signals:

public slots:
    void Tick(double Mt, double u, double dt);
};

#endif // DCMOTOR_H
