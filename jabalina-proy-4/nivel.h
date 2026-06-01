#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QWidget>

class Nivel : public QObject {
    Q_OBJECT

public:
    Nivel(QWidget *parent = nullptr);
    virtual ~Nivel();

    virtual void inicializar() = 0;
    virtual void mostrar() = 0;
    virtual void ocultar() = 0;
    virtual bool haTerminado() = 0;

protected:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timer;
    int tiempoRestante;
};

#endif
