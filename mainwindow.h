#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsTextItem>
#include "nivel2.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void actualizar();

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timer;
    QGraphicsEllipseItem *atleta;
    float atletaX, atletaY;
    float angulo;
    float fuerza;
    QGraphicsLineItem *lineaAngulo;
    QGraphicsTextItem *textoAngulo;
    QGraphicsTextItem *textoFuerza;
    QGraphicsLineItem *jabalina;
    float jabX, jabY;
    float jabVx, jabVy;
    bool jabEnVuelo;
    QGraphicsTextItem *textoDistancia;
    // ARES-1
    float distanciaJugador;
    float distanciaAres;
    float fuerzaAres;
    bool turnoAres;
    QGraphicsEllipseItem *ares;
    QGraphicsTextItem *textoRonda;
    QGraphicsTextItem *textoResultado;
    int ronda;
    // Jabalina de ARES-1
    QGraphicsLineItem *jabAres;
    float jabAresX, jabAresY;
    float jabAresVx, jabAresVy;
    bool jabAresEnVuelo;
    QGraphicsTextItem *textoAres;

    QGraphicsEllipseItem *objetivo;
    float objetivoT;
    Nivel2 *nivel2;
    int rondasGanadas;
    int rondasPerdidas;
};


#endif
