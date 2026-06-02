#include "juego.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    QApplication a(argc, argv);
    Juego juego;
    juego.show();  // ← inicia el juego en pantalla.
    juego.iniciar();
    return a.exec();
}
