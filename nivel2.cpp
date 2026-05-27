#include "nivel2.h"
#include <QMouseEvent>

Nivel2::Nivel2(QWidget *parent) : QObject(parent)
{

    // Escena y vista
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 500);

    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->viewport()->installEventFilter(this);

    // Fondo negro
    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Estrellas
    for(int i = 0; i < 80; i++) {
        int x = rand() % 800;
        int y = rand() % 500;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Jugador en el centro
    jugadorX = 385;
    jugadorY = 225;
    jugador = escena->addEllipse(jugadorX, jugadorY, 30, 30,
                                 QPen(Qt::white),
                                 QBrush(QColor(200, 220, 255)));

    // Jabalina
    jabEnVuelo = false;
    jabalina = escena->addLine(0, 0, 0, 0, QPen(QColor(255, 200, 0), 3));
    jabalina->setVisible(false);

    // Variables
    tiempoRestante = 45;
    vidas = 3;
    velocidadMet = 2.0f;

    // Textos
    textoTimer = escena->addText("Tiempo: 45");
    textoTimer->setDefaultTextColor(Qt::white);
    textoTimer->setPos(350, 10);

    textoVidas = escena->addText("Vidas: 3");
    textoVidas->setDefaultTextColor(Qt::red);
    textoVidas->setPos(10, 10);

    textoResultado = escena->addText("");
    textoResultado->setDefaultTextColor(Qt::yellow);
    textoResultado->setPos(300, 460);

    // Timers
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);
    timer->start(16);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel2::reducirTiempo);
    timerJuego->start(1000);

    vista->hide();
}



Nivel2::~Nivel2()
{
}

bool Nivel2::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!jabEnVuelo) {
            float dx = mouseEvent->pos().x() - (jugadorX + 15);
            float dy = mouseEvent->pos().y() - (jugadorY + 15);
            float dist = sqrt(dx*dx + dy*dy);
            jabVx = (dx/dist) * 8.0f;
            jabVy = (dy/dist) * 8.0f;
            jabX = jugadorX + 15;
            jabY = jugadorY + 15;
            jabEnVuelo = true;
            jabalina->setVisible(true);
        }
    }
    return QObject::eventFilter(obj, event);
}


void Nivel2::mostrar()
{
    vista->show();
    vista->setWindowTitle("Jabalina en la Luna - Nivel 2");
    vista->setWindowFlags(Qt::Window);
    vista->resize(800, 500);
    vista->show();
    timer->start(16);
    timerJuego->start(1000);
    vista->viewport()->installEventFilter(this); // ← agregar aqui también
    vista->show();
    vista->setWindowTitle("Jabalina en la Luna - Nivel 2");
    vista->setWindowFlags(Qt::Window);
    vista->resize(800, 500);
    vista->show();
    timer->start(16);
    timerJuego->start(1000);
}

void Nivel2::ocultar()
{
}

void Nivel2::actualizar()
{
    // Jabalina ← PRIMERO
    if(jabEnVuelo) {
        jabX += jabVx;
        jabY += jabVy;
        jabalina->setLine(jabX, jabY, jabX+15, jabY+15);

        if(jabX < 0 || jabX > 800 || jabY < 0 || jabY > 500) {
            jabEnVuelo = false;
            jabalina->setVisible(false);
        }

        for(int i = 0; i < (int)meteoritos.size(); i++) {
            float dx = jabX - metX[i];
            float dy = jabY - metY[i];
            float dist = sqrt(dx*dx + dy*dy);
            if(dist < 25) {
                escena->removeItem(meteoritos[i]);
                meteoritos.erase(meteoritos.begin() + i);
                metX.erase(metX.begin() + i);
                metY.erase(metY.begin() + i);
                metVx.erase(metVx.begin() + i);
                metVy.erase(metVy.begin() + i);
                jabEnVuelo = false;
                jabalina->setVisible(false);
                break;
            }
        }
    }
    // Mover meteoritos hacia el jugador
    for(int i = 0; i < (int)meteoritos.size(); i++) {
        float dx = jugadorX - metX[i];
        float dy = jugadorY - metY[i];
        float dist = sqrt(dx*dx + dy*dy);

        if(dist > 0) {
            metX[i] += (dx/dist) * velocidadMet;
            metY[i] += (dy/dist) * velocidadMet;
            meteoritos[i]->setPos(metX[i], metY[i]);
        }

        // Colision meteorito con jugador
        if(dist < 30) {
            vidas--;
            textoVidas->setPlainText("Vidas: " + QString::number(vidas));
            // Eliminar meteorito
            escena->removeItem(meteoritos[i]);
            meteoritos.erase(meteoritos.begin() + i);
            metX.erase(metX.begin() + i);
            metY.erase(metY.begin() + i);
            metVx.erase(metVx.begin() + i);
            metVy.erase(metVy.begin() + i);
            i--;

            if(vidas <= 0) {
                timer->stop();
                timerJuego->stop();
                textoResultado->setPlainText("PERDISTE! Sin vidas");
            }
        }
    }
}

void Nivel2::reducirTiempo()
{
    tiempoRestante--;
    textoTimer->setPlainText("Tiempo: " + QString::number(tiempoRestante));

    // Generar meteorito cada 3 segundos
    if(tiempoRestante % 3 == 0) {
        generarMeteorito();
        velocidadMet += 0.2f;
    }

    if(tiempoRestante <= 0) {
        timer->stop();
        timerJuego->stop();
        textoResultado->setPlainText("SOBREVIVISTE! Ganaste el juego!");
    }
}


void Nivel2::generarMeteorito()
{
    if(tiempoRestante <= 0) return;

    // Generar desde un borde aleatorio
    float x, y;
    int borde = rand() % 4;
    if(borde == 0) { x = rand() % 800; y = 0; }
    else if(borde == 1) { x = rand() % 800; y = 500; }
    else if(borde == 2) { x = 0; y = rand() % 500; }
    else { x = 800; y = rand() % 500; }

    QGraphicsEllipseItem* met = escena->addEllipse(0, 0, 25, 25,
                                                   QPen(Qt::NoPen),
                                                   QBrush(QColor(150, 80, 20)));
    met->setPos(x, y);
    met->setZValue(5);

    meteoritos.push_back(met);
    metX.push_back(x);
    metY.push_back(y);
    metVx.push_back(0);
    metVy.push_back(0);
}
