/*#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}*/


#include "juego.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    QApplication a(argc, argv);
    Juego juego;
    juego.show();  // ← agregar esto
    juego.iniciar();
    return a.exec();
}
