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

// Nivel 2: Defensa Estelar
// Vista cenital - el jugador defiende su posicion de meteoritos
// Hereda de Nivel e implementa sus metodos virtuales puros
class Nivel2 : public Nivel {
    Q_OBJECT

public:
    Nivel2(QWidget *parent = nullptr);
    ~Nivel2();  // Destructor libera memoria de todos los meteoritos
    void inicializar() override;
    void mostrar() override;
    void ocultar() override;
    bool haTerminado() override;
    bool eventFilter(QObject *obj, QEvent *event) override; // Captura mouse y teclado
    void setDificultad(Dificultad d);  // Configura velocidad inicial de meteoritos
    void keyPress(QKeyEvent *event);   // Mueve al jugador con flechas

private slots:
    void actualizar();         // Bucle principal ~60fps
    void reducirTiempo();      // Cuenta regresiva y aumenta dificultad
    void generarMeteorito();   // Crea meteorito desde borde aleatorio

private:
    QTimer *timerJuego;  // Timer de 1 segundo para la cuenta regresiva

    // Jugador centrado en pantalla
    QGraphicsEllipseItem *jugador;     // Colision del jugador
    float jugadorX, jugadorY;          // Posicion actual
    QGraphicsPixmapItem *jugadorPixmap; // Sprite cenital

    // Contenedores STL: vector de punteros a Meteorito
    // Se usa vector porque la cantidad cambia en tiempo de ejecucion
    // Memoria dinamica: new en generarMeteorito(), delete en actualizar() y destructor
    vector<Meteorito*> meteoritosObj;  // Objetos logicos con FISICA 4
    vector<QGraphicsEllipseItem*> meteoritos; // Sprites visuales
    vector<float> metX;  // Posiciones X para colisiones
    vector<float> metY;  // Posiciones Y para colisiones

    // Jabalina del jugador - lanzamiento con clic del mouse
    QGraphicsLineItem *jabalina;
    float jabX, jabY;
    float jabVx, jabVy;
    bool jabEnVuelo;
    float tiempoJuego;  // Acumula tiempo para oscilacion de meteoritos

    // Estado del nivel
    int vidas;           // Vidas disponibles
    float velocidadMet;  // Velocidad actual de meteoritos (aumenta con el tiempo)
    bool terminado;

    // Textos informativos
    QGraphicsTextItem *textoTimer;
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoResultado;
    QGraphicsPixmapItem *jugadorPixmap2;
    QGraphicsPixmapItem *jabPixmapN2;

    // Sonido de explosion al destruir meteoritos
    QSoundEffect *sonidoExplosion;
};

#endif
