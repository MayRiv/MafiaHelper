#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T19:44:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MafiaHelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nightdialog.cpp \
    votedialog.cpp \
    warningbutton.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    nightdialog.h \
    votedialog.h \
    warningbutton.h \
    player.h

FORMS    += mainwindow.ui \
    nightdialog.ui \
    votedialog.ui

CONFIG += mobility
MOBILITY = 

