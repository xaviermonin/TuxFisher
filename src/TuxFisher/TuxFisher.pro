# Projet TuxFisher

NAME = TuxFisher
LEADER = "Xavier Monin"
LICENSE = "GPLv2"
EMAIL = "tuxfisher@free.fr"
DESCRIPTION = "Jeu d'arcade/réflexion 2D"

SOURCES += main.cpp \
	widget_tuxfisher.cpp \
	moteur_tuxfisher.cpp \
	mainwindow_tuxfisher.cpp \
	../Commun/choixniveaudialog_tuxfisher.cpp \
	telechargerniveaudialog.cpp \
	gestionson_tuxfisher.cpp \
	saviezvousdialog.cpp \
	../Commun/gestionniveau_tuxfisher.cpp

HEADERS += widget_tuxfisher.h \
	moteur_tuxfisher.h \
	mainwindow_tuxfisher.h \
	../Commun/choixniveaudialog_tuxfisher.h \
	telechargerniveaudialog.h \
	gestionson_tuxfisher.h \
	../Commun/gestionniveau_tuxfisher.h \
	saviezvousdialog.h \
	../Commun/definitions.h

OBJECTS_DIR = .build

CONFIG += release warn_on ressources static

RESOURCES += ressources_tuxfisher.qrc

TRANSLATIONS = app_en.ts

TEMPLATE = app

MOC_DIR = .build

RCC_DIR = .build

win32 {
	DESTDIR = Data
	LIBS += -lOpenAL32 -lalut -llibogg -lvorbis -lvorbisfile
	RC_FILE = TuxFisher.rc
}
unix {
	LIBS += -lopenal -lalut -logg -lvorbis -lvorbisfile
	# PKGCONFIG += ogg ??
	target.path = /usr/bin/tuxfisher
	data.path = /usr/bin/tuxfisher
	data.files = Data/*
	INSTALLS += target data
}

QT += network opengl

TARGET = TuxFisher
