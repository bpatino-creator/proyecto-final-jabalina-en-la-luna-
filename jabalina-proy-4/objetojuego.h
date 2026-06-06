#ifndef OBJETOJUEGO_H
#define OBJETOJUEGO_H

#include <QGraphicsScene>

// Clase base abstracta de la capa logica
// No hereda de Qt - es una clase propia pura
// No se puede instanciar directamente por tener metodo virtual puro
class ObjetoJuego {
public:
    ObjetoJuego(float x, float y);
    virtual ~ObjetoJuego();

    // Metodo virtual puro - hace la clase abstracta
    // Todas las clases hijas DEBEN implementarlo
    // Permite polimorfismo: ObjetoJuego* puede apuntar a Atleta, Jabalina o Meteorito
    virtual void actualizar(float dt) = 0;

protected:
    float x, y;   // Posicion en la escena
    float vx, vy; // Velocidades (usadas en fisica parabolica)
    bool activo;  // Estado del objeto - si esta activo en la escena
};

#endif
