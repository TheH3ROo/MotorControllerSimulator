#ifndef RACECAR_H
#define RACECAR_H

#include "DCMotor.h"
#include "PICtrler.h"
#include "Accumulator.h"
#include <QObject>
#include <QTimer>
#include <QDebug>




/** @brief Elektromos versenyautót szimuláló osztály.
 *
 * Akkumulátort, DC motort és szabályozót tartalmaz.
 * A különféle elemek állapotát a Get... kezdetű függvényekkel lehet lekérni.
 * Két timert is tartalmaz (a motorhoz és a szabályozóhoz), ezek signaljai indítják a számításokat.
 * A konstrukorban adhatóak meg ezek a paraméterek.
*/
class RaceCar : public QObject
{
    Q_OBJECT

    /** A versenyautó állapotát leíró enumeráció.
     * stop - a jármű áll
     * hven - az akku kontaktora meghúz, így a DC sín feszültség alá kerül
     * dren - hajtás engedélyezés
    */
    enum raceCarState{
        stop, hven, dren
    };

public:
    /** Konstruktor
     * @param dtm  A motorhoz tartozó diszkrét időlépcső [ms]. Ilyen időközönként számítódik a motor árama, nyomatéke stb.
     * @param dtpi A szabályozóhoz tartozó diszkrét időlépcső [ms]. Ilyen időközönként számítódik a motorra kapcsolt feszültség értéke.
     * @param D    A jármű kerekének átmérője [m]
     * @param C    A légellenállás figyelembevételéhez szükséges konstans [N/(m/2)^2], F=C*v^2 képlet alapján.
     * @param m    A jármű tömege.
    */
    RaceCar(QObject *parent=0, double dtm=1,
            double dtpi=5, double D=0.2,
            double C=10, double m=200);
    /** Visszatér a pillanatnyi sebességgel.*/
    double GetSpeed(){return vpre;}
    /** Visszatér a pillanatnyi szögsebességgel.*/
    double GetAngSpeed(){return motor.GetW();}
    /** Visszatér a pillanatnyi nyomatékkal.*/
    double GetTorq(){return (motor.GetM()+C*vpre*vpre);}
    /** Visszatér a pillanatnyi motorárammal.*/
    double GetCurr(){return motor.GetI();}
    /** Visszatér az akkumulátor maradék kapacitással.*/
    double GetCapac(){return accu.GetAh();}
    /** Visszatér az akkupakk feszültséggel.*/
    double GetVbat(){return accu.GetVbat();}
    /** Visszatér a DC sín feszültséggel.*/
    double GetVrail(){return accu.GetVrail();}
    /** Visszatér a sebességalapjel értékével.*/
    double GetVref(){return vref;}
    /** Visszatér a cellafeszültségeket tartalmazó vektorral.*/
    const QVector<double>& GetVCell(){return accu.GetVCell();}
    /** Visszatér az autó állapotával.
     * @returns 10, ha az állapot "stop", egyébként 0
    */
    double IsStop();
    /** Visszatér az autó állapotával.
     * @returns 10, ha az állapot "hven", egyébként 0
    */
    double IsHVEN();
    /** Visszatér az autó állapotával.
     * @returns 10, ha az állapot "dren", egyébként 0
    */
    double IsDREN();

private:
    /** Időzítők a motorhoz és a szabályozóhoz.*/
    QTimer motTmr, PITmr;
    /** Példányosított objektumok.*/
    DCMotor motor;
    PICtrler ctrler;
    Accumulator accu;
    /** Az autó állapotát tároló enumeráció példányosítása.*/
    raceCarState state;
    /** A PI szabályozó és a DC motor közötti számítokhoz tartozó változók.*/
    double dtm, dtpi, D, C, m, vpre;
    /** Alapjelek*/
    double Mref, vref;

    /** Frissíti a belső változókat az állapot alapján.
     * @param M    nyomatékalapjel
     * @note M jelenleg nem támogatott
     * @param v    sebességalapjel
    */
    void UpdateState(double M, double v);
    /** Önteszt. Lekérdezni az akkumulátor, a motor és a szabályzó állapotát, majd elküldi a kliensnek.
     * A kliens "selftest" üzenetére hívja meg a DataProc() függvény.
     * @see RaceCar::DataProc()
    */
    void SelfTest();
    /** Debughoz használható függvény. A konzolra kiírja a privát változókat.*/
    void PrintDataToDebug();

signals:
    /** A motorhoz kell bekötni, ennek hatására kell az új motorparamétereket számolni. @see DCMotor::Tick*/
    void MotorTick(double Mt, double u, double dt);
    /** A szabályozóhoz kell bekötni, ennek hatására kell a szabályozót lefuttatni. @see PICtrler::Tick*/
    void PITick(double uBase, double uSense, double Ts, double umax);
    /** Utasítja az akkumulátort, hogy számolja újra a kapacitását, ugyanis energiát vettünk ki belőle.
     * @param i     A kinyert áram nagysága.
     * @param dt    A kinyerés időtartama.
     */
    void AccuUpdate(double i, double dt);
    /** Stringet ad át a bekötött slotnak.
     * Debugra, selftest üzenetek küldésére használható.
     */
    void sendString(const QString&);

private slots:
    /** Kiszámítja a szükséges változókat és átadja azt a motornak, hogy számítsa újra a saját paramétereit.
     * A motor timer timeout() signalját érdemes ide bekötni.
    */
    void MotTimeout();
    /** Kiszámítja a szükséges változókat és átadja azt a szabályzónak, hogy számítsa újra a saját paramétereit.
     * A szabályzó timer timeout() signalját érdemes ide bekötni.
    */
    void PITimeout();

public slots:
    /** A dataParser objektum DataParser::dataReady(...) jelével kell összekötni.
     * @param data  Az adatokat tároló asszociatív tároló. @see DataParser::dataMap
     * @param code  A jelentés-kódot tároló asszociatív tároló. @see DataParser::codeMap
    */
    void DataProc(QMap<quint16, double>& data, QMap<QString, quint16>& code);
};

#endif // RACECAR_H
