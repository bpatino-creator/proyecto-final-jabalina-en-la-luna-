#include "atleta.h"

// Constructor del Atleta
// Inicializa posicion, atributos y crea el sprite en la escena
Atleta::Atleta(float x, float y, QGraphicsScene *escena)
    : ObjetoJuego(x, y)  // Llama al constructor de la clase base
{
    angulo = 45.0f;  // Angulo inicial optimo de lanzamiento
    fuerza = 50.0f;  // Fuerza inicial al 50%
    vidas = 3;       // Vidas iniciales
    fuerte = true;   // Caracteristica del personaje

    // Sprite transparente - el pixmap visual se maneja en Nivel1
    // Se usa QGraphicsEllipseItem para las colisiones
    sprite = escena->addEllipse(x, y, 30, 50,
                                QPen(Qt::NoPen),
                                QBrush(Qt::transparent));
    sprite->setVisible(false);
}

// Destructor - Qt libera el sprite automaticamente por parentesco
Atleta::~Atleta() {}

// Implementacion del metodo virtual puro de ObjetoJuego
// El atleta no tiene logica de actualizacion propia - la maneja Nivel1
void Atleta::actualizar(float /*dt*/) {}

// Actualiza la posicion del atleta y su sprite en la escena
void Atleta::setPosicion(float nx, float ny) {
    this->x = nx;
    this->y = ny;
    sprite->setPos(nx, ny);
}
