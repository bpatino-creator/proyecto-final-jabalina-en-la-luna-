#include "atleta.h"

Atleta::Atleta(float x, float y, QGraphicsScene *escena)
    : ObjetoJuego(x, y)
{
    angulo = 45.0f;
    fuerza = 50.0f;
    vidas = 3;
    fuerte = true;

    /*sprite = escena->addEllipse(x, y, 30, 50,
                                QPen(Qt::white),
                                QBrush(QColor(200, 220, 255)));*/
    sprite = escena->addEllipse(x, y, 30, 50,
                                QPen(Qt::NoPen),
                                QBrush(Qt::transparent));
    sprite->setVisible(false);
}

Atleta::~Atleta() {}

void Atleta::actualizar(float /*dt*/) {}

void Atleta::setPosicion(float nx, float ny) {
    this->x = nx;
    this->y = ny;
    sprite->setPos(nx, ny);
}
