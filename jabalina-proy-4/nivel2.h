#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "meteorito.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <vector>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "dificultad.h"
#include <QSoundEffect>


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
    void setDificultad(Dificultad d);

private slots:
    void actualizar();
    void reducirTiempo();
    void generarMeteorito();

private:
    QTimer *timerJuego;

    QGraphicsEllipseItem *jugador;
    float jugadorX, jugadorY;

    vector<Meteorito*> meteoritosObj;
    vector<QGraphicsEllipseItem*> meteoritos;
    vector<float> metX;
    vector<float> metY;

    QGraphicsLineItem *jabalina;
    float jabX, jabY;
    float jabVx, jabVy;
    bool jabEnVuelo;
    float tiempoJuego;

    int vidas;
    QGraphicsTextItem *textoTimer;
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoResultado;
    QGraphicsPixmapItem *jugadorPixmap;
    QGraphicsPixmapItem *jabPixmapN2;

    //sonido
    QSoundEffect *sonidoExplosion;

    float velocidadMet;
    bool terminado;
};

#endif
