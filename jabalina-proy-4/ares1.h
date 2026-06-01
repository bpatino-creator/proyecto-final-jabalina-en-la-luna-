#ifndef ARES1_H
#define ARES1_H

#include "atleta.h"
#include <vector>
using namespace std;

class Ares1 : public Atleta {
public:
    Ares1(float x, float y, QGraphicsScene *escena);
    ~Ares1();

    void percibir(float distJugador);
    void razonar();
    float actuar();
    void aprender(float resultado);

    float getFuerza() { return fuerza; }

private:
    vector<float> historial;
    float nivelDificultad;
    bool modoAgresivo;
};

#endif
