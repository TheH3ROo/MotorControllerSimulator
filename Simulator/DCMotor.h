#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <QObject>
#include <QDebug>

/** @brief DC motort szimuláló osztály.
 *
 * A Tick() slot hatására kiszámítja az új paramétereket, megvalósít egy szimulációs lépésközt.
 * A Get...() függvényekkel lehet lekérdezni az aktuális állapotokat.
 */

class DCMotor : public QObject
{
    Q_OBJECT
public:
    /** Konstruktor.
     * @param kf    motorállandó szorozva a fluxussal (k*fi).
     * @param R     Rézellenállás.
     * @param L     Motor induktivitása.
     * @param theta Motor tehetetlenségi nyomatéka.
    */
    DCMotor(QObject *parent = 0,
            double kf=6, double R=0.01,
            double L=0.001, double theta=500);
    /**Visszatér a motor áramával.*/
    double GetI(){return i;}
    /**Visszatér a motor szögsebességvéle.*/
    double GetW(){return w;}
    /**Visszatér a motor nyomatékával.
     * @bug A motor alapértelemezett paraméterei miatt túl nagy a számított nyomaték, irreaális: ezért leosztom 100-al.
     */
    double GetM(){return (kf*i/100);}
    /**Motor állapot lekérdezése.
     * @returns true, ha minden rendben.
    */
    bool isOk(){return true;}

private:
    /** Szimulációval számított értékek.*/
    double i, ipre;
    double w, wpre;
    double M;

    /** DC motor paraméterek.*/
    const double kf;
    const double R,L;
    const double theta;

public slots:
    /** Hatására lefut egy paraméterszámítás, egy szimulációs lépésköz.
     * @param Mt    Terhelő nyomaték, tipikusan a súrlódás, légellenállás, gyorsítás miatt. [Nm]
     * @param u     Motorra kapcsolt feszültség pillanatértéke. [V]
     * @param dt    Két hívás közötti idő [s].
    */
    void Tick(double Mt, double u, double dt);
};

#endif // DCMOTOR_H
