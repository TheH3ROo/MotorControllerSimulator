#ifndef PICTRLER_H
#define PICTRLER_H

#include <QObject>

/** @brief PI szabályozót megvalósító osztály.
 *
 * Minden Tick(..) slot híváskor lefut, a GetValue() függvénnyel lekérhető a kimenet értéke.
*/
class PICtrler : public QObject
{
    Q_OBJECT
public:
    /** Konstruktor.
     * @param Ap    A szabályozó erősítése.
     * @param Ti    Integrálási idő [sec].
    */
    explicit PICtrler(QObject *parent = 0,
                      double Ap = 5,
                      double Ti = 1);
    /** A szabályozó kimenetével tér vissza.*/
    double GetValue(){return out;}
    /** SelfTest függvény.
     * @returns true, ha minden rendben.
    */
    bool isOk(){return true;}

private:
    const double Ap, Ti;
    double integral, xpre, out;

public slots:
    /** Ez a slot futtatja le a szabályzási algoritmust.
     * @param uBase     Alapjel bemenet.
     * @param uSense    Érzékelt, szabályozni kívánt jel.
     * @param Ts        Két hívás között eltelt idő [sec]
     * @param umax      Maximális DC feszültség. A kimenet +/- Umax/2 között változhat.
    */
    void Tick(double uBase, double uSense, double Ts, double umax);
};

#endif // PICTRLER_H
