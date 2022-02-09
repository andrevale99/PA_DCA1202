#-------------------------------------------------
#
# Project created by QtCreator 2018-06-11T18:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mensageiro2_cliente
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mensageiro2_cliente_connect.cpp \
    mensageiro2_cliente_main.cpp \
    mysocket.cpp \
    mensageiro2_cliente_thread.cpp

HEADERS  += \
    mensageiro2_cliente_connect.h \
    mensageiro2_cliente_main.h \
    mysocket.h \
    mensageiro2_cliente_thread.h

FORMS    += \
    mensageiro2_cliente_connect.ui \
    mensageiro2_cliente_main.ui

LIBS   += \
       -lpthread
   # -lWs2_32

