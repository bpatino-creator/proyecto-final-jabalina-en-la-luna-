#include "meteorito.h"

// Constructor del Meteorito
// Crea el sprite del meteorito con pixmap y colision transparente
Meteorito::Meteorito(float x, float y, QGraphicsScene *escena)
    : ObjetoJuego(x, y)  // Llama al constructor de la clase base
{
    tamano = 1;  // Tamaño del meteorito
    danio = 1;   // Danio que causa al jugador

    // Carga el sprite visual del meteorito desde recursos Qt
    QPixmap pixMet(":/imagenes/meteorito.png.png");
    pixMet = pixMet.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Elipse transparente para colisiones - el pixmap es el visual
    sprite = escena->addEllipse(0, 0, 40, 40, QPen(Qt::NoPen), QBrush(Qt::transparent));
    sprite->setPos(x, y);
    sprite->setZValue(5);  // Capa de renderizado

    // El pixmap es hijo del sprite - se mueve junto con el
    QGraphicsPixmapItem *metPixmap = escena->addPixmap(pixMet);
    metPixmap->setParentItem(sprite);  // Herencia de posicion Qt
    metPixmap->setZValue(6);
}

// Destructor - Qt libera el sprite y sus hijos automaticamente
Meteorito::~Meteorito() {}

// Implementacion del metodo virtual puro de ObjetoJuego
// El meteorito no usa dt - su movimiento lo maneja Nivel2
void Meteorito::actualizar(float /*dt*/) {}

// FISICA 4: Movimiento dirigido hacia el jugador
// Calcula la direccion normalizada y avanza con la velocidad dada
// Esto permite que el meteorito siempre se dirija al jugador
void Meteorito::moverHacia(float px, float py, float velocidad) {
    float dx = px - x;  // Diferencia en X hacia el jugador
    float dy = py - y;  // Diferencia en Y hacia el jugador
    float dist = sqrt(dx*dx + dy*dy);  // Distancia total
    if(dist > 0) {
        // Normalizamos el vector y multiplicamos por velocidad
        x += (dx/dist) * velocidad;
        y += (dy/dist) * velocidad;
        sprite->setPos(x, y);
    }
}

// Elimina el meteorito de la escena y marca como inactivo
// Memoria dinamica: el delete lo hace Nivel2 despues de llamar eliminar()
void Meteorito::eliminar(QGraphicsScene *escena) {
    escena->removeItem(sprite);
    activo = false;
}
