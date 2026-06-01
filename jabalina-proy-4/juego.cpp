#include "juego.h"

Juego::Juego(QWidget *parent) : QMainWindow(parent)
{
    puntaje = 0;
    nivel1 = new Nivel1(this);
    nivel2 = new Nivel2(this);
    nivelActual = nivel1;

    connect(nivel1, &Nivel1::nivelTerminado, this, &Juego::onNivel1Terminado);
}

Juego::~Juego() {}

void Juego::iniciar()
{
    nivel1->mostrar();
}

void Juego::cambiarNivel(int n)
{
    nivelActual->ocultar();
    if(n == 2) {
        nivelActual = nivel2;
        nivel2->mostrar();
    }
}

void Juego::onNivel1Terminado()
{
    cambiarNivel(2);
}

void Juego::keyPressEvent(QKeyEvent *event)
{
    if(nivelActual == nivel1)
        nivel1->keyPress(event);
}
