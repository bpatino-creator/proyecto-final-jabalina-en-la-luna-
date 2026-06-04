#include "juego.h"
#include <QMessageBox>
#include <QPushButton>

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
    mostrarMenuDificultad();
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

void Juego::mostrarMenuDificultad()
{
    QMessageBox msg;
    msg.setWindowTitle("Jabalina en la Luna");
    msg.setText("Selecciona la dificultad:");
    QPushButton *facil = msg.addButton("Facil", QMessageBox::AcceptRole);
    QPushButton *normal = msg.addButton("Normal", QMessageBox::AcceptRole);
    msg.addButton("Dificil", QMessageBox::AcceptRole);
    msg.exec();

    if(msg.clickedButton() == facil) dificultad = FACIL;
    else if(msg.clickedButton() == normal) dificultad = NORMAL;
    else dificultad = DIFICIL;

    nivel1->setDificultad(dificultad);
    nivel2->setDificultad(dificultad);
}
