#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QWidget>

// Clase abstracta base para los niveles del juego
// Hereda de QObject para usar señales y slots de Qt (capa GUI)
// No se puede instanciar directamente - define la interfaz comun de los niveles
class Nivel : public QObject {
    Q_OBJECT

public:
    Nivel(QWidget *parent = nullptr);
    virtual ~Nivel();

    // Metodos virtuales puros - cada nivel los implementa diferente
    // Polimorfismo: Juego usa Nivel* para manejar Nivel1 y Nivel2 igual
    virtual void inicializar() = 0;      // Prepara el nivel
    virtual void mostrar() = 0;          // Muestra la ventana del nivel
    virtual void ocultar() = 0;          // Oculta la ventana del nivel
    virtual bool haTerminado() = 0;      // Indica si el nivel termino

protected:
    // Atributos compartidos por todos los niveles - capa GUI Qt
    QGraphicsScene *escena;   // Lienzo donde se dibujan los objetos
    QGraphicsView *vista;     // Camara que muestra la escena al jugador
    QTimer *timer;            // Temporizador del bucle principal (~60fps)
    int tiempoRestante;       // Tiempo disponible en el nivel
};

#endif
