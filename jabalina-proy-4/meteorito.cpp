#include "meteorito.h"

Meteorito::Meteorito(float x, float y, QGraphicsScene *escena)
    : ObjetoJuego(x, y)
{
    tamano = 1;
    danio = 1;
    /*sprite = escena->addEllipse(0, 0, 25, 25,
                                QPen(Qt::NoPen),
                                QBrush(QColor(150, 80, 20)));
    sprite->setPos(x, y);
    sprite->setZValue(5);*/
    QPixmap pixMet(":/imagenes/meteorito.png.png");
    pixMet = pixMet.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprite = escena->addEllipse(0, 0, 40, 40, QPen(Qt::NoPen), QBrush(Qt::transparent));
    sprite->setPos(x, y);
    sprite->setZValue(5);
    QGraphicsPixmapItem *metPixmap = escena->addPixmap(pixMet);
    metPixmap->setParentItem(sprite);
    metPixmap->setZValue(6);
}

Meteorito::~Meteorito() {}

void Meteorito::actualizar(float /*dt*/) {}

void Meteorito::moverHacia(float px, float py, float velocidad) {
    float dx = px - x;
    float dy = py - y;
    float dist = sqrt(dx*dx + dy*dy);
    if(dist > 0) {
        x += (dx/dist) * velocidad;
        y += (dy/dist) * velocidad;
        sprite->setPos(x, y);
    }
}

void Meteorito::eliminar(QGraphicsScene *escena) {
    escena->removeItem(sprite);
    activo = false;
}
