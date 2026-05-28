#ifndef ATLETA_H
#define ATLETA_H

#include "objetojuego.h"
#include <QGraphicsEllipseItem>

class Atleta : public ObjetoJuego {
public:
    Atleta(float x, float y, QGraphicsScene *escena);
    virtual ~Atleta();
    virtual void actualizar(float dt) override;

    void setAngulo(float a) { angulo = a; }
    void setFuerza(float f) { fuerza = f; }
    float getAngulo() { return angulo; }
    float getFuerza() { return fuerza; }
    void setVisible(bool v) { sprite->setVisible(v); }
    void setPos(float x, float y);

protected:
    float angulo;
    float fuerza;
    int vidas;
    bool fuerte;
    QGraphicsEllipseItem *sprite;
};

#endif
