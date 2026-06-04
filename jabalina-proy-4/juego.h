#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QKeyEvent>
#include "nivel1.h"
#include "nivel2.h"
#include "dificultad.h"


class Juego : public QMainWindow {
    Q_OBJECT

public:
    Juego(QWidget *parent = nullptr);
    ~Juego();
    void iniciar();
    void cambiarNivel(int n);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onNivel1Terminado();

private:
    Nivel *nivelActual;
    Nivel1 *nivel1;
    Nivel2 *nivel2;
    int puntaje;
    Dificultad dificultad;
    void mostrarMenuDificultad();
};

#endif
