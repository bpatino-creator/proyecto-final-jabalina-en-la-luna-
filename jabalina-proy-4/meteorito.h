#ifndef METEORITO_H
#define METEORITO_H

#include "objetojuego.h"
#include <QGraphicsEllipseItem>
#include <cmath>
#include <QPen>
#include <QBrush>

class Meteorito : public ObjetoJuego {
public:
    Meteorito(float x, float y, QGraphicsScene *escena);
    virtual ~Meteorito();
    virtual void actualizar(float /*dt*/) override;

    void moverHacia(float px, float py, float velocidad);
    float getX() { return x; }
    float getY() { return y; }
    void eliminar(QGraphicsScene *escena);
    QGraphicsEllipseItem* getSprite() { return sprite; }
    void setOscilacion(float ox) {
        x += ox;
        sprite->setPos(x, y);
    }

private:
    QGraphicsEllipseItem *sprite;
    int tamano;
    int danio;
};

#endif
