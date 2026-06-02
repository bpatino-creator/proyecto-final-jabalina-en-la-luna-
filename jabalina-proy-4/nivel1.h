#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "ares1.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QtMath>
#include <ctime>
#include <cstdlib>

class Nivel1 : public Nivel {
    Q_OBJECT

public:
    Nivel1(QWidget *parent = nullptr);
    ~Nivel1();
    void inicializar() override;
    void mostrar() override;
    void ocultar() override;
    bool haTerminado() override;
    void keyPress(QKeyEvent *event);
    void pasarNivel2();

signals:
    void nivelTerminado();

private slots:
    void actualizar();

private:
    // Jugador
    Atleta *atletaObj;
    QGraphicsPixmapItem *atletaPixmap;
    float atletaX, atletaY;
    float angulo, fuerza;

    // Agente
    Ares1 *ares1obj;
    QGraphicsPixmapItem *aresSprite2;

    // Jabalina jugador
    QGraphicsLineItem *jabalina;
    QGraphicsLineItem *lineaAngulo;
    float jabX, jabY, jabVx, jabVy;
    bool jabEnVuelo;

    // Jabalina ARES-1
    QGraphicsLineItem *jabAres;
    float jabAresX, jabAresY, jabAresVx, jabAresVy;
    bool jabAresEnVuelo;
    bool turnoAres;

    // Objetivo movil
    QGraphicsEllipseItem *objetivo;
    float objetivoT;

    // Distancias y rondas
    float distanciaJugador, distanciaAres;
    int ronda;
    int rondasGanadas, rondasPerdidas;

    // Textos
    QGraphicsTextItem *textoAngulo;
    QGraphicsTextItem *textoFuerza;
    QGraphicsTextItem *textoDistancia;
    QGraphicsTextItem *textoRonda;
    QGraphicsTextItem *textoResultado;
    QGraphicsTextItem *textoAres;
    QGraphicsPixmapItem *jabPixmap;
    QGraphicsPixmapItem *jabAresPixmap;

    bool terminado;
};

#endif
