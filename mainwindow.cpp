#include "mainwindow.h"
#include <QtMath>
#include <QKeyEvent>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Tamaño de la ventana
    setFixedSize(800, 500);
    setWindowTitle("Jabalina en la Luna");

    // Crear escena y vista
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 800, 500);

    vista = new QGraphicsView(escena, this);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vista);
    vista->setFocusPolicy(Qt::NoFocus);

    // Fondo negro (cielo lunar)
    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Estrellas
    for(int i = 0; i < 80; i++) {
        int x = rand() % 800;
        int y = rand() % 350;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Tierra en el horizonte
    escena->addEllipse(620, 280, 120, 120,
                       QPen(Qt::NoPen),
                       QBrush(QColor(30, 100, 200)));

    // Continentes (manchas verdes)
    escena->addEllipse(635, 295, 35, 25,
                       QPen(Qt::NoPen),
                       QBrush(QColor(30, 140, 50)));
    escena->addEllipse(670, 310, 25, 20,
                       QPen(Qt::NoPen),
                       QBrush(QColor(30, 140, 50)));
    escena->addEllipse(650, 340, 30, 20,
                       QPen(Qt::NoPen),
                       QBrush(QColor(30, 140, 50)));

    // Suelo lunar
    escena->addRect(0, 400, 800, 100,
                    QPen(Qt::NoPen),
                    QBrush(QColor(150, 150, 150)));

    // Atleta (por ahora un rectangulo blanco)
    atletaX = 80;
    atletaY = 350;
    atleta = escena->addEllipse(atletaX, atletaY, 30, 50,
                                QPen(Qt::white),
                                QBrush(QColor(200, 220, 255)));


    // ARES-1 empieza oculto al lado izquierdo
    ares = escena->addEllipse(150, 350, 30, 50,
                              QPen(Qt::red),
                              QBrush(QColor(255, 100, 100)));
    ares->setVisible(false);

    // Variables ARES-1
    fuerzaAres = 30.0f;
    turnoAres = false;
    distanciaJugador = 0;
    distanciaAres = 0;
    ronda = 1;

    // Texto ronda
    textoRonda = escena->addText("Ronda: 1");
    textoRonda->setDefaultTextColor(Qt::white);
    textoRonda->setPos(350, 10);

    // Texto resultado
    textoResultado = escena->addText("");
    textoResultado->setDefaultTextColor(Qt::yellow);
    textoResultado->setPos(300, 460);

    textoAres = escena->addText("ARES-1 fuerza: 30%");
    textoAres->setDefaultTextColor(Qt::red);
    textoAres->setPos(600, 10);

    // Jabalina
    jabEnVuelo = false;
    jabX = atletaX + 15;
    jabY = atletaY + 10;
    jabalina = escena->addLine(jabX, jabY, jabX+20, jabY,
                               QPen(QColor(255, 200, 0), 3));
    jabalina->setVisible(false);

    // Jabalina ARES-1
    jabAresEnVuelo = false;
    jabAresX = 165;
    jabAresY = 360;
    jabAres = escena->addLine(jabAresX, jabAresY, jabAresX+20, jabAresY,
                              QPen(QColor(255, 50, 50), 3));
    jabAres->setVisible(false);

    // Variables iniciales
    angulo = 45.0f;
    fuerza = 50.0f;

    // Linea que muestra el angulo
    lineaAngulo = escena->addLine(atletaX+15, atletaY+10,
                                  atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                                  atletaY+10 - 60*sin(qDegreesToRadians(angulo)),
                                  QPen(QColor(255, 255, 0), 2));

    // Texto angulo y fuerza
    textoAngulo = escena->addText("Angulo: 45°");
    textoAngulo->setDefaultTextColor(Qt::white);
    textoAngulo->setPos(10, 10);
    textoFuerza = escena->addText("Fuerza: 50%");
    textoFuerza->setDefaultTextColor(Qt::white);
    textoFuerza->setPlainText("Fuerza (W/S): " + QString::number(fuerza) + "%");
    textoDistancia = escena->addText("Distancia: 0 m");
    textoDistancia->setDefaultTextColor(Qt::yellow);
    textoDistancia->setPos(10, 50);

    // Objetivo movil
    objetivoT = 0;
    objetivo = escena->addEllipse(0, 0, 40, 40,
                                  QPen(Qt::yellow),
                                  QBrush(QColor(255, 255, 0)));
    objetivo->setPos(QPointF(500.0, 280.0));
    objetivo->setZValue(100);


    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Up && angulo < 85)
        angulo += 5;
    if(event->key() == Qt::Key_Down && angulo > 5)
        angulo -= 5;
    if(event->key() == Qt::Key_W && fuerza < 70)
        fuerza += 5;
    if(event->key() == Qt::Key_S && fuerza > 10)
        fuerza -= 5;

    // Espacio lanza la jabalina
    if(event->key() == Qt::Key_Space && !jabEnVuelo) {
        float v0 = (fuerza / 100.0f) * 40.0f;
        jabVx = v0 * cos(qDegreesToRadians(angulo));
        jabVy = -v0 * sin(qDegreesToRadians(angulo));
        jabX = atletaX + 15;
        jabY = atletaY + 10;
        jabEnVuelo = true;
        jabalina->setVisible(true);
    }

    // Actualizar linea de angulo
    lineaAngulo->setLine(atletaX+15, atletaY+10,
                         atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                         atletaY+10 - 60*sin(qDegreesToRadians(angulo)));

    // Actualizar textos
    textoAngulo->setPlainText("Angulo: " + QString::number(angulo) + "°");
    textoFuerza->setPlainText("Fuerza: " + QString::number(fuerza) + "%");

}

MainWindow::~MainWindow() {}

void MainWindow::actualizar() {
    // Objetivo movil oscilatorio
    objetivoT += 0.05f;
    float objY = 280 + 60 * sin(objetivoT);
    objetivo->setPos(QPointF(500.0, (double)objY));

    // Logica jabalina jugador
    if(jabEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabVy += g * dt;
        jabX += jabVx * dt * 2.6;
        jabY += jabVy * dt * 2.6;
        jabalina->setLine(jabX, jabY, jabX+20, jabY-5);

        // Colision con objetivo
        float dx = jabX - (objetivo->x() + 20);
        float dy = jabY - (objetivo->y() + 20);
        float dist = sqrt(dx*dx + dy*dy);
        if(dist < 35) {
            jabEnVuelo = false;
            jabalina->setVisible(false);
            textoResultado->setPlainText("ATINASTE AL OBJETIVO! +100 puntos");
            distanciaJugador = (jabX - atletaX) / 10.0f;
            atleta->setVisible(false);
            turnoAres = true;
        }

        if(jabY >= 400 || jabX >= 790) {
            jabEnVuelo = false;
            jabalina->setVisible(false);
            float distancia = (jabX - atletaX) / 10.0f;
            distanciaJugador = distancia;
            textoDistancia->setPlainText("Distancia: " +
                                         QString::number(distancia, 'f', 1) + " m");
            atleta->setVisible(false);
            turnoAres = true;
        }
    }

    // Logica jabalina ARES-1
    if(jabAresEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabAresVy += g * dt;
        jabAresX += jabAresVx * dt * 2.6;
        jabAresY += jabAresVy * dt * 2.6;
        jabAres->setLine(jabAresX, jabAresY, jabAresX+20, jabAresY-5);

        if(jabAresY >= 400 || jabAresX >= 790) {
            jabAresEnVuelo = false;
            jabAres->setVisible(false);
            ares->setVisible(false);
            atleta->setVisible(true);
            distanciaAres = (jabAresX - 165) / 10.0f;

            if(distanciaJugador > distanciaAres) {
                // Jugador gana - ARES-1 sube fuerza porque perdio
                textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": GANASTE! Tu: " +
                                             QString::number(distanciaJugador,'f',1) + "m  ARES-1: " +
                                             QString::number(distanciaAres,'f',1) + "m");
                fuerzaAres += 10.0f;
                textoAres->setPlainText("ARES-1 fuerza: " + QString::number(fuerzaAres) + "%");
                if(fuerzaAres > 80.0f) fuerzaAres = 80.0f;
            } else {
                // ARES-1 gana - baja un poco para equilibrar
                textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": PERDISTE! Tu: " +
                                             QString::number(distanciaJugador,'f',1) + "m  ARES-1: " +
                                             QString::number(distanciaAres,'f',1) + "m");
                fuerzaAres -= 5.0f;
                if(fuerzaAres < 20.0f) fuerzaAres = 20.0f;
                textoAres->setPlainText("ARES-1 fuerza: " + QString::number(fuerzaAres) + "%");
            }

        }
    }

    // ARES-1 lanza cuando es su turno
    if(turnoAres && !jabAresEnVuelo) {
        ares->setPos(atletaX, atletaY);  // misma posicion que el atleta
        ares->setVisible(true);
        float v0 = (fuerzaAres / 100.0f) * 40.0f;
        jabAresVx = v0 * cos(qDegreesToRadians(45.0f));
        jabAresVy = -v0 * sin(qDegreesToRadians(45.0f));
        jabAresX = atletaX + 15;  // misma posicion de lanzamiento
        jabAresY = atletaY + 10;
        jabAresEnVuelo = true;
        jabAres->setVisible(true);
        turnoAres = false;
    }
}
