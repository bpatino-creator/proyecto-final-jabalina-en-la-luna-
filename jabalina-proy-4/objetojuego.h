#ifndef OBJETOJUEGO_H
#define OBJETOJUEGO_H

#include <QGraphicsScene>

class ObjetoJuego {
public:
    ObjetoJuego(float x, float y);
    virtual ~ObjetoJuego();
    virtual void actualizar(float dt) = 0;

protected:
    float x, y;
    float vx, vy;
    bool activo;
};

#endif
