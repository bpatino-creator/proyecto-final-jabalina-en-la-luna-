#ifndef ARES1_H
#define ARES1_H

#include "atleta.h"
#include <vector>
using namespace std;

// Agente inteligente ARES-1 - rival del jugador en el Nivel 1
// Hereda de Atleta (herencia propia) que hereda de ObjetoJuego
// Implementa los 4 componentes de un agente inteligente sencillo
class Ares1 : public Atleta {
public:
    Ares1(float x, float y, QGraphicsScene *escena);
    ~Ares1();

    // COMPONENTE 1: PERCEPCION
    // Registra la distancia del jugador en cada ronda
    void percibir(float distJugador);

    // COMPONENTE 2: RAZONAMIENTO
    // Analiza el historial y activa modo agresivo si pierde 3 seguidas
    void razonar();

    // COMPONENTE 3: ACCION
    // Devuelve la fuerza de lanzamiento actual
    // Siempre lanza a 45 grados - angulo optimo para maxima distancia
    float actuar();

    // COMPONENTE 4: APRENDIZAJE
    // Ajusta la fuerza segun el resultado: sube si pierde, baja si gana
    void aprender(float resultado);

    float getFuerza() { return fuerza; }
    void setFuerzaInicial(float f) { fuerza = f; }  // Configura dificultad inicial

private:
    // Contenedor STL: vector<float> para el historial de distancias
    // Se usa vector porque la cantidad de rondas varia en tiempo de ejecucion
    vector<float> historial;
    float nivelDificultad;  // Factor de dificultad del agente
    bool modoAgresivo;      // Se activa al perder 3 rondas seguidas
};

#endif
