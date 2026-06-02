#include "nivel2.h"
#include <QMouseEvent>

Nivel2::Nivel2(QWidget *parent) : Nivel(parent)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 500);

    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->viewport()->installEventFilter(this);

    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Estrellas
    for(int i = 0; i < 80; i++) {
        int x = rand() % 800;
        int y = rand() % 500;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Crateres (vista cenital)
    escena->addEllipse(100, 100, 80, 80, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(600, 300, 60, 60, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(300, 350, 50, 50, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(650, 80, 70, 70, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));

    // Jugador en el centro
    jugadorX = 385;
    jugadorY = 225;
    /*jugador = escena->addEllipse(jugadorX, jugadorY, 30, 30,
                                 QPen(Qt::white), QBrush(QColor(200, 220, 255)));
    jugador->setZValue(10);*/
    QPixmap pixJugador(":/imagenes/jugador_cenital.png.png");
    pixJugador = pixJugador.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jugadorPixmap = escena->addPixmap(pixJugador);
    jugadorPixmap->setPos(jugadorX, jugadorY);
    jugadorPixmap->setZValue(10);
    jugador = escena->addEllipse(jugadorX, jugadorY, 30, 30,
                                 QPen(Qt::NoPen), QBrush(Qt::transparent));

    // Jabalina
    /*jabEnVuelo = false;
    jabalina = escena->addLine(0, 0, 0, 0, QPen(QColor(255, 200, 0), 3));
    jabalina->setVisible(false);*/


    QPixmap pixJabN2(":/imagenes/jabalina_cenital.png.png");
    pixJabN2 = pixJabN2.scaled(20, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jabPixmapN2 = escena->addPixmap(pixJabN2);
    jabPixmapN2->setPos(0, 0);
    jabPixmapN2->setZValue(10);
    jabPixmapN2->setVisible(false);
    //jabalina->setZValue(10);

    // Variables
    tiempoRestante = 45;
    vidas = 3;
    velocidadMet = 2.0f;
    terminado = false;

    // Textos
    textoTimer = escena->addText("Tiempo: 45");
    textoTimer->setDefaultTextColor(Qt::white);
    textoTimer->setPos(350, 10);
    textoTimer->setZValue(20);

    textoVidas = escena->addText("Vidas: 3");
    textoVidas->setDefaultTextColor(Qt::red);
    textoVidas->setPos(10, 10);
    textoVidas->setZValue(20);

    textoResultado = escena->addText("");
    textoResultado->setDefaultTextColor(Qt::yellow);
    textoResultado->setPos(250, 230);
    textoResultado->setZValue(20);

    // Instruccion
    QGraphicsTextItem *instruccion = escena->addText("Clic para lanzar jabalina");
    instruccion->setDefaultTextColor(QColor(150, 150, 150));
    instruccion->setPos(280, 460);

    // Timers
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel2::reducirTiempo);

    vista->hide();
}

Nivel2::~Nivel2() {}

void Nivel2::inicializar() {}

bool Nivel2::haTerminado() {
    return terminado;
}

bool Nivel2::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!jabEnVuelo) {
            float dx = mouseEvent->pos().x() - (jugadorX + 15);
            float dy = mouseEvent->pos().y() - (jugadorY + 15);
            float dist = sqrt(dx*dx + dy*dy);
            if(dist > 0) {
                jabVx = (dx/dist) * 8.0f;
                jabVy = (dy/dist) * 8.0f;
                jabX = jugadorX + 15;
                jabY = jugadorY + 15;
                jabEnVuelo = true;
                jabPixmapN2->setVisible(true);
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void Nivel2::mostrar()
{
    vista->viewport()->installEventFilter(this);
    vista->setWindowTitle("Jabalina en la Luna - Nivel 2: Defensa Estelar");
    vista->setWindowFlags(Qt::Window);
    vista->resize(800, 500);
    vista->show();
    timer->start(16);
    timerJuego->start(1000);
}

void Nivel2::ocultar()
{
    timer->stop();
    timerJuego->stop();
    vista->hide();
}

void Nivel2::actualizar()
{
    if(terminado) return;

    // Jabalina
    if(jabEnVuelo) {
        jabX += jabVx;
        jabY += jabVy;
        jabPixmapN2->setPos(jabX, jabY);

        if(jabX < 0 || jabX > 800 || jabY < 0 || jabY > 500) {
            jabEnVuelo = false;
            jabPixmapN2->setVisible(false);
        }

        // Colision jabalina con meteoritos
        for(int i = 0; i < (int)meteoritosObj.size(); i++) {
            float dx = jabX - meteoritosObj[i]->getX();
            float dy = jabY - meteoritosObj[i]->getY();
            float dist = sqrt(dx*dx + dy*dy);
            if(dist < 25) {
                meteoritosObj[i]->eliminar(escena);
                delete meteoritosObj[i];
                meteoritosObj.erase(meteoritosObj.begin() + i);
                metX.erase(metX.begin() + i);
                metY.erase(metY.begin() + i);
                jabEnVuelo = false;
                jabPixmapN2->setVisible(false);
                break;
            }
        }
    }

    // Mover meteoritos hacia jugador
    for(int i = 0; i < (int)meteoritosObj.size(); i++) {
        meteoritosObj[i]->moverHacia(jugadorX + 15, jugadorY + 15, velocidadMet);
        metX[i] = meteoritosObj[i]->getX();
        metY[i] = meteoritosObj[i]->getY();

        // Colision meteorito con jugador
        float dx = metX[i] - (jugadorX + 15);
        float dy = metY[i] - (jugadorY + 15);
        float dist = sqrt(dx*dx + dy*dy);
        if(dist < 30) {
            vidas--;
            textoVidas->setPlainText("Vidas: " + QString::number(vidas));
            meteoritosObj[i]->eliminar(escena);
            delete meteoritosObj[i];
            meteoritosObj.erase(meteoritosObj.begin() + i);
            metX.erase(metX.begin() + i);
            metY.erase(metY.begin() + i);
            i--;

            if(vidas <= 0) {
                timer->stop();
                timerJuego->stop();
                textoResultado->setPlainText("GAME OVER! Sin vidas");
                terminado = true;
            }
        }
    }
}

void Nivel2::reducirTiempo()
{
    tiempoRestante--;
    textoTimer->setPlainText("Tiempo: " + QString::number(tiempoRestante));

    if(tiempoRestante % 3 == 0) {
        generarMeteorito();
        velocidadMet += 0.2f;
    }

    if(tiempoRestante <= 0) {
        timer->stop();
        timerJuego->stop();
        textoResultado->setPlainText("SOBREVIVISTE! Ganaste el juego!");
        terminado = true;
    }
}

void Nivel2::generarMeteorito()
{
    if(tiempoRestante <= 0) return;

    float x, y;
    int borde = rand() % 4;
    if(borde == 0) { x = rand() % 800; y = 0; }
    else if(borde == 1) { x = rand() % 800; y = 500; }
    else if(borde == 2) { x = 0; y = rand() % 500; }
    else { x = 800; y = rand() % 500; }

    Meteorito *met = new Meteorito(x, y, escena);
    meteoritosObj.push_back(met);
    metX.push_back(x);
    metY.push_back(y);
}
