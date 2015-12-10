#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
#include <QObject>
#include <QVector>


/** @brief LiIon akkumulátor csomagot megvalósító osztály.
 *
 * A slot segítségével csökkenthető a kapacitása a kivett árammal, a Get...() függvényekkel az állapot kérhető le.
*/
class Accumulator: public QObject
{
    Q_OBJECT

/** Kontaktor állapotát leképző enumeráció.*/
enum contactorState
{
    closed, opened
};

public:
    Accumulator();
    /** Visszatér a teljes csomag feszültségével.*/
    double GetVbat(){return u;}
    /** Visszatér a DC sín feszültségével. Csak akkor egyezik a meg az akku feszültséggel, ha a kontaktor meghúzott.*/
    double GetVrail(){return urail;}
    /** Visszatér az akku fennmaradó kapacitásával.*/
    double GetAh(){return ah;}
    /** Visszatér az akku celláinak feszültségvektorával.*/
    const QVector<double>& GetVCell(){return ucell;}
    /** Kontaktor manipulálása.
     * @param en    Engedélyezés. true=meghúzás, false=elengedés.
     * @returns en, ha sikerült a módosítás. !en, ha nem sikerült a módosítás.
    */
    bool HVEN(bool en);
    /** Állapot lekérdezés.
     * @returns true, ha az akku rendben. false, ha az akku lemerült.
    */
    bool isOk();

private:
    double umax, umin, ah, u, urail, cellnum;
    QVector<double> ucell, celldiff;
    contactorState contactor;

    /** Cellafeszültségek újraszámolása, ha változott a fennmaradó kapacitás.*/
    void UpdateCells();

public slots:
    /** Ennek hatására fog a kivett áram és idő arányában csökkenni a kapacitás.
     * @param i     Vételezett áram nagysága.
     * @param dt    Áramfelvétel időtartama.
     * @see RaceCar::UpdateCapacity
    */
    void UpdateCapacity(double i, double dt);
};

#endif // ACCUMULATOR_H
