#include "nivel2.h"
#include <QMouseEvent>
#include <QDebug>

// Constructor del Nivel 2: Defensa Estelar
// Vista cenital - el jugador defiende su posicion de meteoritos
Nivel2::Nivel2(QWidget *parent) : Nivel(parent)
{
    // Escena y vista grafica
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 500);
    tiempoJuego = 0;  // Contador de tiempo para la oscilacion de meteoritos

    vista = new QGraphicsView(escena, (QWidget*)parent);
    vista->setFixedSize(800, 500);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->viewport()->installEventFilter(this);  // Captura mouse y teclado

    // Fondo oscuro lunar
    escena->setBackgroundBrush(QBrush(QColor(5, 5, 20)));

    // Sonido de explosion al destruir meteoritos
    sonidoExplosion = new QSoundEffect(this);
    sonidoExplosion->setSource(QUrl("qrc:/sonidos/explosion.wav.wav"));
    sonidoExplosion->setVolume(0.8f);

    // Estrellas de fondo generadas aleatoriamente
    for(int i = 0; i < 80; i++) {
        int x = rand() % 800;
        int y = rand() % 500;
        escena->addEllipse(x, y, 2, 2, QPen(Qt::white), QBrush(Qt::white));
    }

    // Crateres lunares - decoracion de la vista cenital
    escena->addEllipse(100, 100, 80, 80, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(600, 300, 60, 60, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(300, 350, 50, 50, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));
    escena->addEllipse(650, 80, 70, 70, QPen(QColor(100,100,100), 2), QBrush(QColor(30,30,30)));

    // Jugador centrado en la pantalla con sprite cenital
    jugadorX = 385;
    jugadorY = 225;
    QPixmap pixJugador(":/imagenes/jugador_cenital.png.png");
    pixJugador = pixJugador.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jugadorPixmap = escena->addPixmap(pixJugador);
    jugadorPixmap->setPos(jugadorX, jugadorY);
    jugadorPixmap->setZValue(10);

    // Elipse transparente para colisiones del jugador
    jugador = escena->addEllipse(jugadorX, jugadorY, 30, 30,
                                 QPen(Qt::NoPen), QBrush(Qt::transparent));

    // Jabalina cenital - lanzamiento con clic del mouse
    QPixmap pixJabN2(":/imagenes/jabalina_cenital.png.png");
    pixJabN2 = pixJabN2.scaled(20, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    jabPixmapN2 = escena->addPixmap(pixJabN2);
    jabPixmapN2->setPos(0, 0);
    jabPixmapN2->setZValue(10);
    jabPixmapN2->setVisible(false);

    // Variables del nivel
    tiempoRestante = 45;  // 45 segundos para sobrevivir
    vidas = 3;            // 3 vidas disponibles
    velocidadMet = 1.0f;  // Velocidad inicial de meteoritos (se ajusta con dificultad)
    terminado = false;
    jabEnVuelo = false;

    // Textos informativos en pantalla
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

    QGraphicsTextItem *instruccion = escena->addText("Clic para lanzar jabalina");
    instruccion->setDefaultTextColor(QColor(150, 150, 150));
    instruccion->setPos(280, 460);

    // Timer de juego a 60fps aprox
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);

    // Timer de cuenta regresiva - 1 segundo
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel2::reducirTiempo);

    vista->hide();
}

// Destructor: libera la memoria dinamica de todos los meteoritos
// Uso de memoria dinamica: new en generarMeteorito(), delete aqui
Nivel2::~Nivel2()
{
    for(int i = 0; i < (int)meteoritosObj.size(); i++) {
        delete meteoritosObj[i];
    }
    meteoritosObj.clear();
}

void Nivel2::inicializar() {}

bool Nivel2::haTerminado() {
    return terminado;
}

// Filtro de eventos: captura clics del mouse y teclas del teclado
bool Nivel2::eventFilter(QObject *obj, QEvent *event)
{
    // Manejo de teclas para mover al jugador
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        keyPress(keyEvent);
        return true;
    }

    // Clic del mouse: lanza la jabalina hacia donde se hizo clic
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!jabEnVuelo) {
            // Calcula direccion normalizada del jugador al punto de clic
            float dx = mouseEvent->pos().x() - (jugadorX + 15);
            float dy = mouseEvent->pos().y() - (jugadorY + 15);
            float dist = sqrt(dx*dx + dy*dy);
            if(dist > 0) {
                jabVx = (dx/dist) * 8.0f;  // Velocidad horizontal
                jabVy = (dy/dist) * 8.0f;  // Velocidad vertical
                jabX = jugadorX + 15;
                jabY = jugadorY + 15;
                jabEnVuelo = true;
                jabPixmapN2->setVisible(true);
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

// Muestra el nivel 2 en una ventana separada
void Nivel2::mostrar()
{
    vista->viewport()->installEventFilter(this);
    vista->setWindowTitle("Jabalina en la Luna - Nivel 2: Defensa Estelar");
    vista->setWindowFlags(Qt::Window);
    vista->resize(800, 500);
    vista->show();
    timer->start(16);       // ~60fps
    timerJuego->start(1000); // 1 segundo
    vista->setFocus();
    vista->grabKeyboard();
    vista->installEventFilter(this);
}

void Nivel2::ocultar()
{
    timer->stop();
    timerJuego->stop();
    vista->hide();
}

// Bucle principal del Nivel 2 - se ejecuta ~60 veces por segundo
void Nivel2::actualizar()
{
    tiempoJuego += 0.016f;  // Acumula tiempo para la oscilacion
    if(terminado) return;

    // Logica de la jabalina del jugador
    if(jabEnVuelo) {
        jabX += jabVx;  // Movimiento rectilineo en X
        jabY += jabVy;  // Movimiento rectilineo en Y
        jabPixmapN2->setPos(jabX, jabY);

        // Si sale de pantalla desaparece
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
                // Destruye el meteorito - memoria dinamica
                meteoritosObj[i]->eliminar(escena);
                delete meteoritosObj[i];
                sonidoExplosion->play();  // Sonido de explosion
                // Elimina del vector STL
                meteoritosObj.erase(meteoritosObj.begin() + i);
                metX.erase(metX.begin() + i);
                metY.erase(metY.begin() + i);
                jabEnVuelo = false;
                jabPixmapN2->setVisible(false);
                break;
            }
        }
    }

    // Logica de meteoritos con excepcion y fisica oscilatorio compuesta
    try {
        for(int i = 0; i < (int)meteoritosObj.size(); i++) {
            // Excepcion: verifica que el meteorito no sea nulo
            if(meteoritosObj[i] == nullptr)
                throw std::runtime_error("Meteorito nulo");

            // FISICA 4: Movimiento oscilatorio compuesto
            // Combina movimiento dirigido al jugador + oscilacion lateral con sin()
            meteoritosObj[i]->moverHacia(jugadorX + 15, jugadorY + 15, velocidadMet);
            float oscilacion = sin(tiempoJuego * 0.15f + i * 1.5f) * 15.0f;
            meteoritosObj[i]->setOscilacion(oscilacion);  // Zigzag lateral

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
                delete meteoritosObj[i];  // Libera memoria
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
    } catch(std::runtime_error &e) {
        // Captura error si hay un meteorito nulo en el vector
        qDebug() << e.what();
    }
}

// Reduce el tiempo cada segundo y aumenta la dificultad gradualmente
void Nivel2::reducirTiempo()
{
    tiempoRestante--;
    textoTimer->setPlainText("Tiempo: " + QString::number(tiempoRestante));

    // Cada 5 segundos genera un nuevo meteorito y aumenta velocidad
    if(tiempoRestante % 5 == 0) {
        generarMeteorito();
        velocidadMet += 0.2f;  // Dificultad progresiva
    }

    // Condicion de victoria: sobrevivir 45 segundos
    if(tiempoRestante <= 0) {
        timer->stop();
        timerJuego->stop();
        textoResultado->setPlainText("SOBREVIVISTE! Ganaste el juego!");
        terminado = true;
    }
}

// Genera un meteorito desde un borde aleatorio de la pantalla
// Usa memoria dinamica: new Meteorito - delete en actualizar() o destructor
void Nivel2::generarMeteorito()
{
    if(tiempoRestante <= 0) return;

    // Posicion aleatoria en uno de los 4 bordes
    float x, y;
    int borde = rand() % 4;
    if(borde == 0) { x = rand() % 800; y = 0; }        // Borde superior
    else if(borde == 1) { x = rand() % 800; y = 500; } // Borde inferior
    else if(borde == 2) { x = 0; y = rand() % 500; }   // Borde izquierdo
    else { x = 800; y = rand() % 500; }                  // Borde derecho

    // Crea meteorito con new - se almacena en vector<Meteorito*> STL
    Meteorito *met = new Meteorito(x, y, escena);
    meteoritosObj.push_back(met);  // Agrega al contenedor STL
    metX.push_back(x);
    metY.push_back(y);
}

// Mueve al jugador con las teclas de flecha
void Nivel2::keyPress(QKeyEvent *event) {
    float velocidad = 5.0f;
    if(event->key() == Qt::Key_Up && jugadorY > 0) jugadorY -= velocidad;
    if(event->key() == Qt::Key_Down && jugadorY < 470) jugadorY += velocidad;
    if(event->key() == Qt::Key_Left && jugadorX > 0) jugadorX -= velocidad;
    if(event->key() == Qt::Key_Right && jugadorX < 770) jugadorX += velocidad;
    jugador->setPos(jugadorX, jugadorY);
    jugadorPixmap->setPos(jugadorX, jugadorY);
}

// Configura la velocidad inicial de meteoritos segun la dificultad elegida
void Nivel2::setDificultad(Dificultad d) {
    if(d == FACIL) {
        velocidadMet = 0.8f;   // Meteoritos lentos
    } else if(d == NORMAL) {
        velocidadMet = 1.2f;   // Velocidad media
    } else {
        velocidadMet = 1.5f;   // Meteoritos rapidos
    }
}
