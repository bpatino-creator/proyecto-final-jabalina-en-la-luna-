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
#include "dificultad.h"
#include <QSoundEffect>

// Nivel 1: Entrenamiento contra ARES-1
// Vista lateral - competencia de lanzamiento de jabalina en la Luna
// Hereda de Nivel e implementa sus metodos virtuales puros
class Nivel1 : public Nivel {
    Q_OBJECT

public:
    Nivel1(QWidget *parent = nullptr);
    ~Nivel1();
    void inicializar() override;
    void mostrar() override;
    void ocultar() override;
    bool haTerminado() override;
    void keyPress(QKeyEvent *event);   // Maneja entrada del teclado
    void pasarNivel2();                // Evalua resultado y transiciona al nivel 2
    void setDificultad(Dificultad d);  // Configura dificultad de ARES-1

signals:
    // Señal Qt emitida cuando terminan las 3 rondas
    // Juego la conecta para saber cuando cambiar al Nivel 2
    void nivelTerminado();

private slots:
    void actualizar();  // Bucle principal ~60fps - private porque solo lo llama el timer

private:
    // Jugador - clase propia Atleta (hereda de ObjetoJuego)
    Atleta *atletaObj;              // Logica del atleta
    QGraphicsPixmapItem *atletaPixmap; // Sprite visual
    float atletaX, atletaY;        // Posicion en la escena
    float angulo, fuerza;          // Parametros de lanzamiento

    // Agente ARES-1 - clase propia (hereda de Atleta)
    Ares1 *ares1obj;               // Agente inteligente con 4 componentes
    QGraphicsPixmapItem *aresSprite2; // Sprite visual de ARES-1

    // Jabalina jugador - fisica parabolica con g=1.62 m/s²
    QGraphicsLineItem *jabalina;   // Linea de referencia (no usada visualmente)
    QGraphicsLineItem *lineaAngulo; // Indicador visual del angulo
    float jabX, jabY;              // Posicion actual de la jabalina
    float jabVx, jabVy;            // Velocidades (componentes del vector)
    bool jabEnVuelo;               // Estado del lanzamiento

    // Jabalina ARES-1 - misma fisica parabolica
    QGraphicsLineItem *jabAres;
    float jabAresX, jabAresY, jabAresVx, jabAresVy;
    bool jabAresEnVuelo;
    bool turnoAres;  // Indica si es el turno del agente

    // Objetivo movil - FISICA 2: movimiento oscilatorio con sin()
    QGraphicsEllipseItem *objetivo;
    float objetivoT;  // Parametro de tiempo para la funcion seno

    // Resultados de las rondas
    float distanciaJugador, distanciaAres;
    int ronda;
    int rondasGanadas, rondasPerdidas;

    // Sonidos del nivel
    QSoundEffect *sonidoFondo;       // Musica de fondo en loop
    QSoundEffect *sonidoLanzamiento; // Sonido al lanzar

    // Textos informativos en pantalla
    QGraphicsTextItem *textoAngulo;
    QGraphicsTextItem *textoFuerza;
    QGraphicsTextItem *textoDistancia;
    QGraphicsTextItem *textoRonda;
    QGraphicsTextItem *textoResultado;
    QGraphicsTextItem *textoAres;
    QGraphicsPixmapItem *jabPixmap;
    QGraphicsPixmapItem *jabAresPixmap;
    QGraphicsPixmapItem *objetivoPixmap;
    QGraphicsPixmapItem *nubePixmap;

    // Nube de polvo cosmico - FISICA 3: perturbacion aleatoria
    QGraphicsEllipseItem *nubePolvo;
    float nubeX, nubeY;   // Posicion de la nube
    bool nubeActiva;      // Si la nube esta visible
    int nubeTimer;        // Contador para desaparecer

    bool terminado;  // Estado del nivel
};

#endif
