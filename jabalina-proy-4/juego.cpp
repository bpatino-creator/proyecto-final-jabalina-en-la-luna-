#include "juego.h"

Juego::Juego(QWidget *parent) : QMainWindow(parent)
{
    puntaje = 0;
    nivel1 = new Nivel1(this);
    nivel2 = new Nivel2(this);
    nivelActual = nivel1;
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

void Juego::keyPressEvent(QKeyEvent *event)
{
    nivel1->keyPress(event);
}
