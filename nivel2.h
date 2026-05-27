#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QGraphicsTextItem>
#include <vector>
#include <QMouseEvent>


using namespace std;

class Nivel2 : public QObject {
    Q_OBJECT

public:
    Nivel2(QWidget *parent = nullptr);
    ~Nivel2();
    void mostrar();
    void ocultar();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void actualizar();

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timer;
    QTimer *timerJuego;

    // Jugador
    QGraphicsEllipseItem *jugador;
    float jugadorX, jugadorY;

    // Meteoritos
    vector<QGraphicsEllipseItem*> meteoritos;
    vector<float> metX;
    vector<float> metY;
    vector<float> metVx;
    vector<float> metVy;

    // Jabalina
    QGraphicsLineItem *jabalina;
    float jabX, jabY;
    float jabVx, jabVy;
    bool jabEnVuelo;

    // Timer juego
    int tiempoRestante;
    int vidas;
    QGraphicsTextItem *textoTimer;
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoResultado;

    float velocidadMet;

private slots:
    void reducirTiempo();
    void generarMeteorito();
};

#endif
