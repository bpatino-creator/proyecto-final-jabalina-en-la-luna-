#include "atleta.h"

Atleta::Atleta(float x, float y, QGraphicsScene *escena)
    : ObjetoJuego(x, y)
{
    angulo = 45.0f;
    fuerza = 50.0f;
    vidas = 3;
    fuerte = true;

    sprite = escena->addEllipse(x, y, 30, 50,
                                QPen(Qt::white),
                                QBrush(QColor(200, 220, 255)));
}

Atleta::~Atleta() {}

void Atleta::actualizar(float /*dt*/) {}

void Atleta::setPos(float x, float y) {
    this->x = x;
    this->y = y;
    sprite->setPos(x, y);
}
