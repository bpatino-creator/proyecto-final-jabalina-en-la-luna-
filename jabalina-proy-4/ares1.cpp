#include "ares1.h"

// Constructor del agente ARES-1
// Inicializa el agente con posicion, escena y valores por defecto
Ares1::Ares1(float x, float y, QGraphicsScene *escena)
    : Atleta(x, y, escena)
{
    nivelDificultad = 0.3f;  // Nivel de dificultad inicial
    modoAgresivo = false;     // Modo agresivo desactivado al inicio
    fuerza = 15.0f;           // Fuerza inicial de lanzamiento

    // Sprite transparente - el pixmap se maneja en Nivel1
    sprite->setBrush(QBrush(Qt::transparent));
    sprite->setPen(QPen(Qt::NoPen));
    sprite->setVisible(false);
}

Ares1::~Ares1() {}

// COMPONENTE 1: PERCEPCION
// El agente percibe la distancia del jugador en cada ronda
// y la almacena en el historial para aprender de ella
void Ares1::percibir(float distJugador) {
    historial.push_back(distJugador);  // vector<float> STL - cantidad variable de datos
}

// COMPONENTE 2: RAZONAMIENTO
// Analiza el historial y activa modo agresivo si pierde 3 rondas seguidas
void Ares1::razonar() {
    if(historial.size() >= 3) {
        int perdidasSeguidas = 0;
        // Revisa las ultimas 3 entradas del historial
        for(int i = historial.size()-1; i >= 0 && i >= (int)historial.size()-3; i--) {
            if(historial[i] > 0) perdidasSeguidas++;
        }
        // Si perdio 3 seguidas activa modo agresivo
        if(perdidasSeguidas >= 3) modoAgresivo = true;
    }
}

// COMPONENTE 3: ACCION
// Devuelve la fuerza actual para el lanzamiento
// ARES-1 siempre lanza a 45 grados - angulo optimo para maxima distancia
float Ares1::actuar() {
    return fuerza;
}

// COMPONENTE 4: APRENDIZAJE
// Ajusta la fuerza segun el resultado de la ronda anterior
// Si gana el jugador (resultado > 0): ARES-1 sube su fuerza
// Si pierde el jugador (resultado <= 0): ARES-1 baja ligeramente
void Ares1::aprender(float resultado) {
    if(resultado > 0) {
        // Jugador gano - ARES-1 sube fuerza mas rapido si esta en modo agresivo
        fuerza += modoAgresivo ? 30.0f : 15.0f;
        if(fuerza > 100.0f) fuerza = 100.0f;  // Limite maximo
    } else {
        // ARES-1 gano - baja ligeramente para equilibrar el juego
        fuerza -= 1.0f;
        if(fuerza < 10.0f) fuerza = 10.0f;  // Limite minimo
        modoAgresivo = false;
    }
    razonar();  // Actualiza el razonamiento despues de cada aprendizaje
}
