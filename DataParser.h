#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include <QErrorMessage>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>
#include <QDateTime>
#include <QQueue>
#include <QtGlobal>

/** @brief Az kapott adat értelmezéséért, szétbontásáért felelős objektum.
 *
 * A dataInput(...) slotban kapott streamből a saját protokollnak megfeleően
 * elmenti a vett adatokat a dataMap változóba.
 * A dataMap kulcsa 16bites szám, amely egyedi minden adathoz, értéke double szám.
 * Azt, hogy mely kódhoz milyen jelentés tartozik a codeMap változó tartalmazza,
 * segítségével olvashatóbbá válik a forráskód, hiszen nem számokkal, hanem stringekkel
 * lehet a dataMap tagokra hivatkozni.
 * A program mellett található code.txt tartalmazza a jelentés-kód párokat, a konstruktor
 * abból a fájlból olvassa ki ezeket a hozzárendeléseket, ez alapján töltődnek fel a
 * codeMap és dataMap változók.
 * A code.txt fájl elemeinek módosításával újabb adatokat lehet felvenni a dataMap változóba,
 * így könnyen bővíthetővé válik a szoftver.
 * @warning A szerver és kliens oldalon is meg kell egyezzen a code.txt fájl!
*/
class DataParser : public QObject
{
    Q_OBJECT

public:
    /** Konstruktor.
     * @param codeFilePath  A jelentés-kód párokat tartalmazó fájl elérési útja.
    */
    DataParser(const QString& codeFilePath);
    /** Segédfüggvény az adatstring-kód kereséséhez.*/
    quint16 GetCode(QString str){return codeMap[str];}
    /** Hibakereséshez használatos függvény, a konzolra írja a dataMap elemeit. */
    void PrintDataToDebug();

private:
    /** A kód-jelentést tartalmazó txt fájl objektum.*/
    QFile codeFile;
    /** A jelentés-kód párokat tartalmazó asszociatív tároló.*/
    QMap<QString, quint16> codeMap;
    /** Adatokat tartalmazó asszociatív tároló.*/
    QMap<quint16, double> dataMap;

signals:
    /** Hibakezelő függ jel.*/
    void errorOccurred(const QString&);
    /** Ez a signal emittálódik ha érkezett adat, és a parsolás elkészült.
     *  Az adatfeldolgozó slothoz érdemes bekötni.*/
    void dataReady(QMap<quint16, double>&, QMap<QString, quint16>&);

public slots:
    /** Ide lehet bekötni a kommunikációs dataReady signalokat,
     *  vagyis jelezni, hogy adat érkezett. A kommunikációs obejktum jeléhez kapcsolható.*/
    void dataInput(QDataStream&);
};

