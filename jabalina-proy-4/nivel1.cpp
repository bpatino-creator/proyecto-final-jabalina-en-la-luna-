#include "nivel1.h"
#include <QDebug>
#include <stdexcept>
#include <QSoundEffect>
#include <QUrl>
#include <QMessageBox>

Nivel1::Nivel1(QWidget *parent) : Nivel(parent)
{
    // Escena y vista
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 1500, 600);
    vista = new QGraphicsView(escena, parent);
    vista->setFixedSize(1500, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);

    // Sonido de fondo
    sonidoFondo = new QSoundEffect(this);
    sonidoFondo->setSource(QUrl("qrc:/sonidos/fondo.wav.wav"));
    sonidoFondo->setLoopCount(QSoundEffect::Infinite);
    sonidoFondo->setVolume(0.3f);

    // Sonido lanzamiento
    sonidoLanzamiento = new QSoundEffect(this);
    sonidoLanzamiento->setSource(QUrl("qrc:/sonidos/lanzamiento.wav.wav"));
    sonidoLanzamiento->setVolume(0.8f);

    // Fondo
    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Estrellas
    for(int i = 0; i < 80; i++) {
        int x = rand() % 1500;
        int y = rand() % 400;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Tierra
    escena->addEllipse(1200, 280, 120, 120, QPen(Qt::NoPen), QBrush(QColor(30, 100, 200)));
    escena->addEllipse(1215, 295, 35, 25, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));
    escena->addEllipse(1250, 310, 25, 20, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));
    escena->addEllipse(1230, 340, 30, 20, QPen(Qt::NoPen), QBrush(QColor(30, 140, 50)));

    // Suelo lunar
    escena->addRect(0, 500, 1500, 100, QPen(Qt::NoPen), QBrush(QColor(150, 150, 150)));

    // Variables
    atletaX = 40;
    atletaY = 420;
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
    nubeActiva = false;
    nubeTimer = 0;
    nubeX = 0;
    nubeY = 0;

    // Atleta
    atletaObj = new Atleta(atletaX, atletaY, escena);
    try {
        QPixmap pixAtleta(":/imagenes/atleta.png.png");
        if(pixAtleta.isNull())
            throw std::runtime_error("Error: no se pudo cargar atleta.png");
        pixAtleta = pixAtleta.scaled(60, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        atletaPixmap = escena->addPixmap(pixAtleta);
        atletaPixmap->setPos(atletaX, atletaY);
        atletaPixmap->setZValue(5);
    } catch(std::runtime_error &e) {
        qDebug() << e.what();
        atletaPixmap = escena->addPixmap(QPixmap());
    }

    // ARES-1
    ares1obj = new Ares1(atletaX, atletaY, escena);
    try {
        QPixmap pixAres(":/imagenes/ares1.png.png");
        if(pixAres.isNull())
            throw std::runtime_error("Error: no se pudo cargar ares1.png");
        pixAres = pixAres.scaled(60, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        aresSprite2 = escena->addPixmap(pixAres);
        aresSprite2->setPos(atletaX, atletaY);
        aresSprite2->setZValue(5);
        aresSprite2->setVisible(false);
    } catch(std::runtime_error &e) {
        qDebug() << e.what();
        aresSprite2 = escena->addPixmap(QPixmap());
        aresSprite2->setVisible(false);
    }

    // Jabalina jugador
    jabX = atletaX + 15;
    jabY = atletaY + 10;
    QPixmap pixJab(":/imagenes/jabalina.png.png");
    pixJab = pixJab.scaled(80, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jabPixmap = escena->addPixmap(pixJab);
    jabPixmap->setPos(jabX, jabY);
    jabPixmap->setZValue(6);
    jabPixmap->setVisible(false);

    // Jabalina ARES-1
    jabAresX = atletaX + 15;
    jabAresY = atletaY + 10;
    QPixmap pixJabAres(":/imagenes/jabalina.png.png");
    pixJabAres = pixJabAres.scaled(80, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jabAresPixmap = escena->addPixmap(pixJabAres);
    jabAresPixmap->setPos(jabAresX, jabAresY);
    jabAresPixmap->setZValue(6);
    jabAresPixmap->setVisible(false);

    // Linea angulo
    lineaAngulo = escena->addLine(atletaX+15, atletaY+10,
                                  atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                                  atletaY+10 - 60*sin(qDegreesToRadians(angulo)),
                                  QPen(QColor(255, 255, 0), 2));

    // Objetivo movil
    QPixmap pixObjetivo(":/imagenes/objetivo.png.png");
    pixObjetivo = pixObjetivo.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    objetivoPixmap = escena->addPixmap(pixObjetivo);
    objetivoPixmap->setPos(800, 300);
    objetivoPixmap->setZValue(100);
    objetivo = escena->addEllipse(0, 0, 60, 60, QPen(Qt::NoPen), QBrush(Qt::transparent));
    objetivo->setPos(800, 300);
    objetivo->setZValue(99);

    // Nube de polvo cosmico
    QPixmap pixNube(":/imagenes/nube.png.png");
    pixNube = pixNube.scaled(150, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    nubePixmap = escena->addPixmap(pixNube);
    nubePixmap->setVisible(false);
    nubePixmap->setZValue(50);
    nubePolvo = escena->addEllipse(0, 0, 150, 80, QPen(Qt::NoPen), QBrush(Qt::transparent));
    nubePolvo->setVisible(false);

    // Textos
    textoAngulo = escena->addText("Angulo: 45");
    textoAngulo->setDefaultTextColor(Qt::white);
    textoAngulo->setPos(10, 10);

    textoFuerza = escena->addText("Fuerza: 50%");
    textoFuerza->setDefaultTextColor(Qt::white);
    textoFuerza->setPos(10, 30);

    textoDistancia = escena->addText("Distancia: 0 m");
    textoDistancia->setDefaultTextColor(Qt::yellow);
    textoDistancia->setPos(10, 50);

    textoRonda = escena->addText("Ronda: 1 de 3");
    textoRonda->setDefaultTextColor(Qt::white);
    textoRonda->setPos(650, 10);

    textoResultado = escena->addText("");
    textoResultado->setDefaultTextColor(Qt::yellow);
    textoResultado->setPos(400, 550);

    textoAres = escena->addText("ARES-1 fuerza: 30%");
    textoAres->setDefaultTextColor(Qt::red);
    textoAres->setPos(1100, 10);

    // Instrucciones
    QGraphicsTextItem *instrucciones = escena->addText("Flechas: angulo | W/S: fuerza | Espacio: lanzar");
    instrucciones->setDefaultTextColor(QColor(150, 150, 150));
    instrucciones->setPos(400, 30);

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
    vista->setWindowTitle("Jabalina en la Luna - Nivel 1: Entrenamiento");
    vista->show();
    timer->start(16);
    sonidoFondo->play();
}

void Nivel1::ocultar() {
    timer->stop();
    sonidoFondo->stop();
    vista->hide();
}

bool Nivel1::haTerminado() {
    return terminado;
}

void Nivel1::pasarNivel2() {
    timer->stop();
    sonidoFondo->stop();

    QString resultado = "=== RESULTADOS NIVEL 1 ===\n\n";
    resultado += "Rondas ganadas: " + QString::number(rondasGanadas) + "\n";
    resultado += "Rondas perdidas: " + QString::number(rondasPerdidas) + "\n\n";

    if(rondasGanadas >= 2 || rondasGanadas == rondasPerdidas)  {
        resultado += "GANASTE el entrenamiento!\nPasas al Nivel 2: Defensa Estelar";
        QMessageBox::information(nullptr, "Fin del Nivel 1", resultado);
        terminado = true;
        emit nivelTerminado();
    } else {
        resultado += "No superaste el entrenamiento!\nDebes ganar al menos 2 rondas.\nVolvemos a intentarlo...";
        QMessageBox::warning(nullptr, "Entrenamiento fallido", resultado);
        // Reiniciar el nivel
        ronda = 1;
        rondasGanadas = 0;
        rondasPerdidas = 0;
        textoRonda->setPlainText("Ronda: 1 de 3");
        timer->start(16);
        sonidoFondo->play();
    }
}

void Nivel1::keyPress(QKeyEvent *event) {
    if(jabEnVuelo || turnoAres) return;

    if(event->key() == Qt::Key_Up && angulo < 85) angulo += 5;
    if(event->key() == Qt::Key_Down && angulo > 5) angulo -= 5;
    if(event->key() == Qt::Key_W && fuerza < 100) fuerza += 5;
    if(event->key() == Qt::Key_S && fuerza > 10) fuerza -= 5;

    if(event->key() == Qt::Key_Space) {
        float v0 = (fuerza / 100.0f) * 40.0f;
        float viento = 0;
        if(nubeActiva) {
            float distNube = abs((atletaX + 15) - (nubeX + 75));
            if(distNube < 400)
                viento = (rand() % 40 - 20) * 0.5f;
        }
        jabVx = v0 * cos(qDegreesToRadians(angulo)) + viento;
        jabVy = -v0 * sin(qDegreesToRadians(angulo));
        jabX = atletaX + 15;
        jabY = atletaY + 10;
        jabEnVuelo = true;
        sonidoLanzamiento->play();
        jabPixmap->setVisible(true);
        ares1obj->percibir(distanciaJugador);
    }

    lineaAngulo->setLine(atletaX+15, atletaY+10,
                         atletaX+15 + 60*cos(qDegreesToRadians(angulo)),
                         atletaY+10 - 60*sin(qDegreesToRadians(angulo)));
    textoAngulo->setPlainText("Angulo: " + QString::number(angulo) + "°");
    textoFuerza->setPlainText("Fuerza: " + QString::number((int)fuerza) + "%");
}

void Nivel1::actualizar() {
    // Objetivo oscilatorio
    objetivoT += 0.015f;
    float objY = 250 + 150 * sin(objetivoT);
    objetivo->setPos(QPointF(800.0, (double)objY));
    objetivoPixmap->setPos(objetivo->pos());

    // Generar nube de polvo aleatoriamente
    if(!turnoAres && (rand() % 300 == 0)) {
        nubeX = 300 + rand() % 900;
        nubeY = 150 + rand() % 250;
        nubePolvo->setPos(nubeX, nubeY);
        nubePixmap->setPos(nubeX, nubeY);
        nubePixmap->setVisible(true);
        nubePolvo->setVisible(true);
        nubeActiva = true;
        nubeTimer = 0;
    }

    // Mover nube suavemente
    if(nubeActiva) {
        nubeTimer++;
        nubeX += 0.8f;
        if(nubeX > 1400) nubeX = 300;
        nubePolvo->setPos(nubeX, nubeY);
        nubePixmap->setPos(nubeX, nubeY);

        if(nubeTimer > 500) {
            nubePolvo->setVisible(false);
            nubePixmap->setVisible(false);
            nubeActiva = false;
            nubeTimer = 0;
        }
    }

    // Jabalina jugador
    if(jabEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabVy += g * dt;
        jabX += jabVx * dt * 1.2f;
        jabY += jabVy * dt * 1.2f;
        jabPixmap->setPos(jabX, jabY);

        // Perturbacion durante el vuelo cerca de la nube
        if(nubeActiva) {
            float dxN = jabX - (nubeX + 75);
            float dyN = jabY - (nubeY + 40);
            float distN = sqrt(dxN*dxN + dyN*dyN);
            if(distN < 130) {
                jabVx += (rand() % 20 - 10) * 0.3f;
            }
        }

        // Colision con objetivo - penalizacion
        float dx = jabX - (objetivo->x() + 30);
        float dy = jabY - (objetivo->y() + 30);
        float dist = sqrt(dx*dx + dy*dy);
        if(dist < 40) {
            jabEnVuelo = false;
            jabPixmap->setVisible(false);
            distanciaJugador = 0;  // pierde la ronda
            textoResultado->setPlainText("TOCASTE EL OBJETO PROHIBIDO! Ronda perdida!");
            atletaPixmap->setVisible(false);
            turnoAres = true;
        }

        if(jabY >= 500 || jabX >= 1490 || jabX < 0) {
            jabEnVuelo = false;
            jabPixmap->setVisible(false);
            distanciaJugador = (jabX - atletaX) / 10.0f;
            if(distanciaJugador < 0) distanciaJugador = 0;
            textoDistancia->setPlainText("Distancia: " +
                QString::number(distanciaJugador, 'f', 1) + " m");
            atletaPixmap->setVisible(false);
            turnoAres = true;
        }
    }

    // Jabalina ARES-1
    if(jabAresEnVuelo) {
        float g = 1.62f;
        float dt = 0.016f;
        jabAresVy += g * dt;
        jabAresX += jabAresVx * dt * 1.2f;
        jabAresY += jabAresVy * dt * 1.2f;
        jabAresPixmap->setPos(jabAresX, jabAresY);

        // Perturbacion nube sobre ARES-1
        if(nubeActiva) {
            float dxN = jabAresX - (nubeX + 75);
            float dyN = jabAresY - (nubeY + 40);
            float distN = sqrt(dxN*dxN + dyN*dyN);
            if(distN < 130) {
                jabAresVx += (rand() % 20 - 10) * 0.3f;
            }
        }

        // Colision ARES-1 con objetivo - penalizacion
        float dxA = jabAresX - (objetivo->x() + 30);
        float dyA = jabAresY - (objetivo->y() + 30);
        float distA = sqrt(dxA*dxA + dyA*dyA);
        if(distA < 40) {
            jabAresEnVuelo = false;
            jabAresPixmap->setVisible(false);
            aresSprite2->setVisible(false);
            atletaPixmap->setVisible(true);
            distanciaAres = (jabAresX - atletaX) / 10.0f;
            distanciaAres -= 50.0f;
            if(distanciaAres < 0) distanciaAres = 0;
            textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": ARES-1 toco zona de polvo!");

            if(distanciaJugador > distanciaAres) {
                textoResultado->setPlainText("Ronda " + QString::number(ronda) + ": GANASTE! Tu: " +
                    QString::number(distanciaJugador,'f',1) + "m  ARES-1: " +
                    QString::number(distanciaAres,'f',1) + "m");
                ares1obj->aprender(1);
                rondasGanadas++;
            } else {
                ares1obj->aprender(-1);
                rondasPerdidas++;
            }
            textoAres->setPlainText("ARES-1 fuerza: " + QString::number((int)(ares1obj->getFuerza())) + "%");
            ronda++;
            textoRonda->setPlainText("Ronda: " + QString::number(ronda) + " de 3");
            if(ronda > 2) pasarNivel2();
        }

        if(jabAresY >= 500 || jabAresX >= 1490 || jabAresX < 0) {
            jabAresEnVuelo = false;
            jabAresPixmap->setVisible(false);
            aresSprite2->setVisible(false);
            atletaPixmap->setVisible(true);
            distanciaAres = (jabAresX - atletaX) / 10.0f;
            if(distanciaAres < 0) distanciaAres = 0;

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
            textoAres->setPlainText("ARES-1 fuerza: " + QString::number((int)(ares1obj->getFuerza())) + "%");
            ronda++;
            textoRonda->setPlainText("Ronda: " + QString::number(ronda) + " de 3");
            if(ronda > 2) pasarNivel2();
        }
    }

    // ARES-1 lanza en su turno
    if(turnoAres && !jabAresEnVuelo) {
        aresSprite2->setPos(atletaX, atletaY);
        aresSprite2->setVisible(true);
        float fuerzaAres = ares1obj->actuar();
        float v0 = (fuerzaAres / 100.0f) * 40.0f;
        float vientoAres = 0;
        if(nubeActiva) {
            float distNube = abs(atletaX - nubeX);
            if(distNube < 400)
                vientoAres = (rand() % 40 - 20) * 0.5f;
        }
        jabAresVx = v0 * cos(qDegreesToRadians(45.0f)) + vientoAres;
        jabAresVy = -v0 * sin(qDegreesToRadians(45.0f));
        jabAresX = atletaX + 15;
        jabAresY = atletaY + 10;
        jabAresEnVuelo = true;
        jabAresPixmap->setVisible(true);
        turnoAres = false;
    }
}

void Nivel1::setDificultad(Dificultad d) {
    if(d == FACIL) {
        ares1obj->setFuerzaInicial(70.0f);
    } else if(d == NORMAL) {
        ares1obj->setFuerzaInicial(80.0f);
    } else {
        ares1obj->setFuerzaInicial(90.0f);
    }
}
