#include "jabalina.h"

Jabalina::Jabalina(float x, float y, float angulo, float fuerza, float viento, QGraphicsScene *escena)
    : ObjetoJuego(x, y)
{
    g = 1.62f;
    enVuelo = true;
    float v0 = (fuerza / 100.0f) * 40.0f;
    vx = v0 * cos(qDegreesToRadians(angulo)) + viento;
    vy = -v0 * sin(qDegreesToRadians(angulo));

    sprite = escena->addLine(x, y, x+20, y-5,
                             QPen(QColor(255, 200, 0), 3));
}

Jabalina::~Jabalina() {}

void Jabalina::actualizar(float dt) {
    if(!enVuelo) return;
    vy += g * dt;
    x += vx * dt * 2.6f;
    y += vy * dt * 2.6f;
    sprite->setLine(x, y, x+20, y-5);

    if(y >= 400 || x >= 790) {
        enVuelo = false;
        sprite->setVisible(false);
    }
}
