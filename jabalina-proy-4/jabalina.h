#ifndef JABALINA_H
#define JABALINA_H

#include "objetojuego.h"
#include <QGraphicsLineItem>
#include <QtMath>
#include <QPen>

class Jabalina : public ObjetoJuego {
public:
    Jabalina(float x, float y, float angulo, float fuerza, float viento, QGraphicsScene *escena);
    virtual ~Jabalina();
    virtual void actualizar(float dt) override;

    bool estaEnVuelo() { return enVuelo; }
    void setVisible(bool v) { sprite->setVisible(v); }
    float getX() { return x; }
    float getY() { return y; }

private:
    bool enVuelo;
    float g;
    QGraphicsLineItem *sprite;
};

#endif
