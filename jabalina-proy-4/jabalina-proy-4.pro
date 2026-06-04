QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ares1.cpp \
    atleta.cpp \
    jabalina.cpp \
    juego.cpp \
    main.cpp \
    meteorito.cpp \
    nivel.cpp \
    nivel1.cpp \
    nivel2.cpp \
    objetojuego.cpp

HEADERS += \
    ares1.h \
    atleta.h \
    dificultad.h \
    jabalina.h \
    juego.h \
    meteorito.h \
    nivel.h \
    nivel1.h \
    nivel2.h \
    objetojuego.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc
