#include "nivel1.h"

Nivel1::Nivel1(QWidget *parent) : Nivel(parent)
{
    // Escena y vista
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 500);
    vista = new QGraphicsView(escena, parent);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);

    // Fondo
    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Estrellas
    for(int i = 0; i < 80; i++) {
        int x = rand() % 800;
        int y = rand() % 350;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Tierra
    escena->addEllipse(620, 280, 120, 120, QPen(Qt::NoPen), QBrush(QColor(30, 100, 200)));
    escena->addEllipse(635, 295, 35, 25, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));
    escena->addEllipse(670, 310, 25, 20, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));
    escena->addEllipse(650, 340, 30, 20, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));

    // Suelo lunar
    escena->addRect(0, 400, 800, 100, QPen(Qt::NoPen), QBrush(QColor(150, 150, 150)));

    // Variables
    atletaX = 40;
    atletaY = 350;
    angulo = 45.0f;
    fuerza = 50.0f;
    jabEnVuelo = false;
    jabAresEnVuelo = false;
    turnoAres = false;
    distanciaJugador = 0;
    distanciaAres = 0;
    ronda = 1;
    rondasGanadas = 0;
    rondasPerdidas = 0;
    terminado = false;
    objetivoT = 0;

    // Atleta usando clase Atleta
    atletaObj = new Atleta(atletaX, atletaY, escena);
    atletaSprite = escena->addEllipse(atletaX, atletaY, 30, 50,
                                      QPen(Qt::white), QBrush(QColor(200, 220, 255)));

    // ARES-1 usando clase Ares1
    ares1obj = new Ares1(150, 350, escena);
    aresSprite = escena->addEllipse(150, 350, 30, 50,
                                    QPen(Qt::red), QBrush(QColor(255, 100, 100)));
    aresSprite->setVisible(false);

    // Jabalina jugador
    jabX = atletaX + 15;
    jabY = atletaY + 10;
    jabalina = escena->addLine(jabX, jabY, jabX+20, jabY, QPen(QColor(255, 200, 0), 3));
    jabalina->setVisible(false);

    // Jabalina ARES-1
    jabAresX = atletaX + 15;
    jabAresY = atletaY + 10;
    jabAres = escena->addLine(jabAresX, jabAresY, jabAresX+20, jabAresY,
                              QPen(QColor(255, 50, 50), 3));
    jabAres->setVisible(false);

    // Linea angulo
    lineaAngulo = escena->addLine(atletaX+15, atletaY+10,
                                  atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                                  atletaY+10 - 60*sin(qDegreesToRadians(angulo)),
                                  QPen(QColor(255, 255, 0), 2));

    // Objetivo movil
    objetivo = escena->addEllipse(0, 0, 40, 40,
                                  QPen(Qt::yellow), QBrush(QColor(255, 255, 0)));
    objetivo->setPos(QPointF(500.0, 280.0));
    objetivo->setZValue(100);

    // Textos
    textoAngulo = escena->addText("Angulo: 45");
    textoAngulo->setDefaultTextColor(Qt::white);
    textoAngulo->setPos(10, 10);

    textoFuerza = escena->addText("Fuerza: 100%");
    textoFuerza->setDefaultTextColor(Qt::white);
    textoFuerza->setPos(10, 30);

    textoDistancia = escena->addText("Distancia: 0 m");
    textoDistancia->setDefaultTextColor(Qt::yellow);
    textoDistancia->setPos(10, 50);

    textoRonda = escena->addText("Ronda: 1");
    textoRonda->setDefaultTextColor(Qt::white);
    textoRonda->setPos(350, 10);

    textoResultado = escena->addText("");
    textoResultado->setDefaultTextColor(Qt::yellow);
    textoResultado->setPos(200, 460);

    textoAres = escena->addText("ARES-1 fuerza: 30%");
    textoAres->setDefaultTextColor(Qt::red);
    textoAres->setPos(600, 10);

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);

    vista->hide();
}

Nivel1::~Nivel1() {
    delete atletaObj;
    delete ares1obj;
}

void Nivel1::inicializar() {}

void Nivel1::mostrar() {
    vista->setWindowTitle("Jabalina en la Luna - Nivel 1");
    vista->show();
    timer->start(16);
}

void Nivel1::ocultar() {
    timer->stop();
    vista->hide();
}

bool Nivel1::haTerminado() {
    return terminado;
}

void Nivel1::pasarNivel2() {
    terminado = true;
    emit nivelTerminado();
}

void Nivel1::keyPress(QKeyEvent *event) {
    if(event->key() == Qt::Key_Up && angulo < 85) angulo += 5;
    if(event->key() == Qt::Key_Down && angulo > 5) angulo -= 5;
    if(event->key() == Qt::Key_W && fuerza < 50) fuerza += 5;
    if(event->key() == Qt::Key_S && fuerza > 10) fuerza -= 5;

    if(event->key() == Qt::Key_Space && !jabEnVuelo && !turnoAres) {
        float v0 = (fuerza / 100.0f) * 40.0f;
        float viento = (rand() % 20 - 10) * 0.3f;
        jabVx = v0 * cos(qDegreesToRadians(angulo)) + viento;
        jabVy = -v0 * sin(qDegreesToRadians(angulo));
        jabX = atletaX + 15;
        jabY = atletaY + 10;
        jabEnVuelo = true;
        jabalina->setVisible(true);

        // Agente percibe la situacion
        ares1obj->percibir(distanciaJugador);
    }

    lineaAngulo->setLine(atletaX+15, atletaY+10,
                         atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                         atletaY+10 - 60*sin(qDegreesToRadians(angulo)));
    textoAngulo->setPlainText("Angulo: " + QString::number(angulo) + "°");
    textoFuerza->setPlainText("Fuerza: " + QString::number((int)(fuerza * 2)) + "%");
}

void Nivel1::actualizar() {
    // Objetivo oscilatorio
    objetivoT += 0.01f;
    float objY = 200 + 150 * sin(objetivoT);
    objetivo->setPos(QPointF(500.0, (double)objY));

    // Jabalina jugador
    if(jabEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabVy += g * dt;
        jabX += jabVx * dt * 2.6f;
        jabY += jabVy * dt * 2.6f;
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
            atletaSprite->setVisible(false);
            turnoAres = true;
        }

        if(jabY >= 400 || jabX >= 790) {
            jabEnVuelo = false;
            jabalina->setVisible(false);
            distanciaJugador = (jabX - atletaX) / 10.0f;
            textoDistancia->setPlainText("Distancia: " +
                QString::number(distanciaJugador, 'f', 1) + " m");
            atletaSprite->setVisible(false);
            turnoAres = true;
        }
    }

    // Jabalina ARES-1
    if(jabAresEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabAresVy += g * dt;
        jabAresX += jabAresVx * dt * 2.6f;
        jabAresY += jabAresVy * dt * 2.6f;
        jabAres->setLine(jabAresX, jabAresY, jabAresX+20, jabAresY-5);

        // Colision ARES-1 con objetivo
        float dxA = jabAresX - (objetivo->x() + 20);
        float dyA = jabAresY - (objetivo->y() + 20);
        float distA = sqrt(dxA*dxA + dyA*dyA);
        if(distA < 35) {
            jabAresEnVuelo = false;
            jabAres->setVisible(false);
            aresSprite->setVisible(false);
            atletaSprite->setVisible(true);
            distanciaAres = (jabAresX - atletaX) / 10.0f;
            textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": ARES-1 ATINO! PERDISTE");
            ares1obj->aprender(-1);
            textoAres->setPlainText("ARES-1 fuerza: " + QString::number((int)(ares1obj->getFuerza() * 2)) + "%");
            ronda++;
            textoRonda->setPlainText("Ronda: " + QString::number(ronda));
            rondasPerdidas++;
            if(ronda > 5) pasarNivel2();
        }

        if(jabAresY >= 400 || jabAresX >= 790) {
            jabAresEnVuelo = false;
            jabAres->setVisible(false);
            aresSprite->setVisible(false);
            atletaSprite->setVisible(true);
            distanciaAres = (jabAresX - atletaX) / 10.0f;

            if(distanciaJugador > distanciaAres) {
                textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": GANASTE! Tu: " +
                    QString::number(distanciaJugador,'f',1) + "m  ARES-1: " +
                    QString::number(distanciaAres,'f',1) + "m");
                ares1obj->aprender(1);
                rondasGanadas++;
            } else {
                textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": PERDISTE! Tu: " +
                    QString::number(distanciaJugador,'f',1) + "m  ARES-1: " +
                    QString::number(distanciaAres,'f',1) + "m");
                ares1obj->aprender(-1);
                rondasPerdidas++;
            }
            textoAres->setPlainText("ARES-1 fuerza: " + QString::number((int)(ares1obj->getFuerza() * 2)) + "%");
            ronda++;
            textoRonda->setPlainText("Ronda: " + QString::number(ronda));
            if(ronda > 5) pasarNivel2();
        }
    }

    // ARES-1 lanza en su turno
    if(turnoAres && !jabAresEnVuelo) {
        aresSprite->setPos(atletaX, atletaY);
        aresSprite->setVisible(true);
        float fuerzaAres = ares1obj->actuar();
        float v0 = (fuerzaAres / 100.0f) * 40.0f;
        jabAresVx = v0 * cos(qDegreesToRadians(45.0f));
        jabAresVy = -v0 * sin(qDegreesToRadians(45.0f));
        jabAresX = atletaX + 15;
        jabAresY = atletaY + 10;
        jabAresEnVuelo = true;
        jabAres->setVisible(true);
        turnoAres = false;
    }
}
