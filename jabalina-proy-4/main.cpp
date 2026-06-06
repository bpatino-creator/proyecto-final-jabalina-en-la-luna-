#include "juego.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    QApplication a(argc, argv);
    Juego juego;
    juego.show();
    juego.iniciar();
    return a.exec();
}
