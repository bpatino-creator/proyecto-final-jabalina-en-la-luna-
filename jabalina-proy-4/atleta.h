#ifndef ATLETA_H
#define ATLETA_H

#include "objetojuego.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>

class Atleta : public ObjetoJuego {
public:
    Atleta(float x, float y, QGraphicsScene *escena);
    virtual ~Atleta();
    virtual void actualizar(float /*dt*/) override;

    void setAngulo(float a) { angulo = a; }
    void setFuerza(float f) { fuerza = f; }
    float getAngulo() { return angulo; }
    float getFuerza() { return fuerza; }
    void setVisible(bool v) { sprite->setVisible(v); }
    void setPosicion(float nx, float ny);

protected:
    float angulo;
    float fuerza;
    int vidas;
    bool fuerte;
    QGraphicsEllipseItem *sprite;
};

#endif
