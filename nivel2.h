#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <vector>

using namespace std;

class Nivel2 : public Nivel {
    Q_OBJECT

public:
    Nivel2(QWidget *parent = nullptr);
    ~Nivel2();
    void inicializar() override;
    void mostrar() override;
    void ocultar() override;
    bool haTerminado() override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void actualizar();
    void reducirTiempo();
    void generarMeteorito();

private:
    QTimer *timerJuego;

    QGraphicsEllipseItem *jugador;
    float jugadorX, jugadorY;

    vector<QGraphicsEllipseItem*> meteoritos;
    vector<float> metX;
    vector<float> metY;
    vector<float> metVx;
    vector<float> metVy;

    QGraphicsLineItem *jabalina;
    float jabX, jabY;
    float jabVx, jabVy;
    bool jabEnVuelo;

    int vidas;
    QGraphicsTextItem *textoTimer;
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoResultado;

    float velocidadMet;
    bool terminado;
};

#endif
