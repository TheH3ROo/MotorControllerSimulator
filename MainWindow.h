#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/** @brief A GUI felület létrehozásáért felelős objektum.
 *
 * Jelenleg csak azért van, hogy egy ablak feljövetele jelezze a szimulátor indulását,
 * funkciója egyenlőre nincsen.
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
