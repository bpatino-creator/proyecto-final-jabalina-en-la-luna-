#include "nivel.h"

Nivel::Nivel(QWidget *parent) : QObject(parent)
{
    escena = nullptr;
    vista = nullptr;
    timer = nullptr;
    tiempoRestante = 0;
}

Nivel::~Nivel() {}
