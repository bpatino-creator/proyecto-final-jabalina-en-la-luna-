#include "ares1.h"

Ares1::Ares1(float x, float y, QGraphicsScene *escena)
    : Atleta(x, y, escena)
{
    nivelDificultad = 0.3f;
    modoAgresivo = false;
    fuerza = 15.0f;

    // Cambiar color a rojo
    sprite->setBrush(QBrush(QColor(255, 100, 100)));
    sprite->setPen(QPen(Qt::red));
}

Ares1::~Ares1() {}

void Ares1::percibir(float distJugador) {
    historial.push_back(distJugador);
}

void Ares1::razonar() {
    if(historial.size() >= 3) {
        int perdidasSeguidas = 0;
        for(int i = historial.size()-1; i >= 0 && i >= (int)historial.size()-3; i--) {
            if(historial[i] > 0) perdidasSeguidas++;
        }
        if(perdidasSeguidas >= 3) modoAgresivo = true;
    }
}

float Ares1::actuar() {
    return fuerza;
}

void Ares1::aprender(float resultado) {
    if(resultado > 0) {
        fuerza += modoAgresivo ? 15.0f : 5.0f;
        if(fuerza > 50.0f) fuerza = 50.0f;
    } else {
        fuerza -= 5.0f;
        if(fuerza < 10.0f) fuerza = 10.0f;
        modoAgresivo = false;
    }
    razonar();
}
