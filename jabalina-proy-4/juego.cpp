#include "juego.h"
#include <QMessageBox>
#include <QPushButton>

// Constructor del controlador principal del juego
// Crea los dos niveles y conecta la señal de transicion entre ellos
Juego::Juego(QWidget *parent) : QMainWindow(parent)
{
    puntaje = 0;

    // Memoria dinamica: se crean los dos niveles con new
    // Qt los destruye automaticamente por parentesco (this como parent)
    nivel1 = new Nivel1(this);
    nivel2 = new Nivel2(this);
    nivelActual = nivel1;  // Empieza en el nivel 1

    // Conexion Qt: cuando Nivel1 emite nivelTerminado, Juego llama onNivel1Terminado
    // Esto desacopla los niveles - Nivel1 no sabe que existe Nivel2
    connect(nivel1, &Nivel1::nivelTerminado, this, &Juego::onNivel1Terminado);
}

// Destructor - Qt libera nivel1 y nivel2 automaticamente por parentesco
Juego::~Juego() {}

// Inicia el juego mostrando el menu de dificultad y arrancando el nivel 1
void Juego::iniciar()
{
    mostrarMenuDificultad();
    nivel1->mostrar();
}

// Cambia entre niveles: oculta el actual y muestra el nuevo
void Juego::cambiarNivel(int n)
{
    nivelActual->ocultar();
    if(n == 2) {
        nivelActual = nivel2;  // Polimorfismo: nivelActual apunta a Nivel2
        nivel2->mostrar();
    }
}

// Slot que responde a la señal nivelTerminado de Nivel1
void Juego::onNivel1Terminado()
{
    cambiarNivel(2);
}

// Redirige las teclas al nivel activo
// Polimorfismo: nivelActual puede ser Nivel1 o Nivel2
void Juego::keyPressEvent(QKeyEvent *event) {
    if(nivelActual == nivel1)
        nivel1->keyPress(event);
    else if(nivelActual == nivel2)
        nivel2->keyPress(event);
}

// Muestra el menu de seleccion de dificultad al inicio
// Configura ambos niveles segun la eleccion del jugador
void Juego::mostrarMenuDificultad()
{
    QMessageBox msg;
    msg.setWindowTitle("Jabalina en la Luna");
    msg.setText("Selecciona la dificultad:");
    QPushButton *facil = msg.addButton("Facil", QMessageBox::AcceptRole);
    QPushButton *normal = msg.addButton("Normal", QMessageBox::AcceptRole);
    msg.addButton("Dificil", QMessageBox::AcceptRole);
    msg.exec();

    // Determina la dificultad segun el boton presionado
    if(msg.clickedButton() == facil) dificultad = FACIL;
    else if(msg.clickedButton() == normal) dificultad = NORMAL;
    else dificultad = DIFICIL;

    // Aplica la dificultad a ambos niveles
    nivel1->setDificultad(dificultad);
    nivel2->setDificultad(dificultad);
}
