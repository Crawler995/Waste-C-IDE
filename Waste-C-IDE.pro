#-------------------------------------------------
#
# Project created by QtCreator 2019-08-26T10:49:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Waste-C-IDE
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


SOURCES += main.cpp\
        mainwindow.cpp \
    components/workarea.cpp \
    components/editorarea.cpp \
    components/editor.cpp \
    components/debuginfoarea.cpp \
    components/runoutputarea.cpp \
    components/welcomepage.cpp \
    components/actionbutton.cpp \
    features/cpphighlighter.cpp

HEADERS  += mainwindow.h \
    features/config.h \
    features/colorboard.h \
    components/workarea.h \
    components/editorarea.h \
    components/editor.h \
    components/debuginfoarea.h \
    components/runoutputarea.h \
    components/welcomepage.h \
    components/actionbutton.h \
    features/cpphighlighter.h
