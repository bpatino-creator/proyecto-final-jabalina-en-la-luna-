#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "atleta.h"
#include "ares1.h"
#include "jabalina.h"
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QKeyEvent>

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

private slots:
    void actualizar();

private:
    Atleta *atleta;
    Ares1 *ares1;

    float atletaX, atletaY;
    float angulo, fuerza;

    QGraphicsLineItem *lineaAngulo;
    QGraphicsLineItem *jabalina;
    float jabX, jabY, jabVx, jabVy;
    bool jabEnVuelo;

    QGraphicsLineItem *jabAres;
    float jabAresX, jabAresY, jabAresVx, jabAresVy;
    bool jabAresEnVuelo;
    bool turnoAres;

    float distanciaJugador, distanciaAres;
    int ronda;
    int rondasGanadas, rondasPerdidas;
    float objetivoT;

    QGraphicsEllipseItem *objetivo;
    QGraphicsTextItem *textoAngulo;
    QGraphicsTextItem *textoFuerza;
    QGraphicsTextItem *textoDistancia;
    QGraphicsTextItem *textoRonda;
    QGraphicsTextItem *textoResultado;
    QGraphicsTextItem *textoAres;

    bool terminado;
};

#endif
