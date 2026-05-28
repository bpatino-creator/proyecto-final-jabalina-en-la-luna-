#include "nivel1.h"
#include <QtMath>
#include <cstdlib>

Nivel1::Nivel1(QWidget *parent) : Nivel(parent)
{
    // Crear escena y vista
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 500);
    vista = new QGraphicsView(escena, parent);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->hide();

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);

    // Variables
    terminado = false;
    ronda = 1;
    rondasGanadas = 0;
    rondasPerdidas = 0;
    turnoAres = false;
    jabEnVuelo = false;
    jabAresEnVuelo = false;
    distanciaJugador = 0;
    distanciaAres = 0;
    angulo = 45.0f;
    fuerza = 50.0f;
    objetivoT = 0;
    atletaX = 40;
    atletaY = 350;
}

Nivel1::~Nivel1() {}

void Nivel1::inicializar() {}

void Nivel1::mostrar() {
    vista->setWindowTitle("Jabalina en la Luna - Nivel 1");
    vista->show();
    timer->start(16);
}

void Nivel1::ocultar() {
    vista->hide();
}

bool Nivel1::haTerminado() {
    return terminado;
}

void Nivel1::keyPress(QKeyEvent */*event*/) {}

void Nivel1::actualizar() {}
