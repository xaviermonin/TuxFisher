NAME = "Editeur de TuxFisher"
LEADER = "Xavier Monin"
LICENSE = "GPLv2"
EMAIL = "tuxfisher@free.fr"
DESCRIPTION = "Editeur de niveaux pour TuxFisher"

HEADERS += mainwindow_editeur.h \
	widget_editeur.h \
	niveaudialog_editeur.h \
	../Commun/gestionniveau_tuxfisher.h \
	cartedialog_editeur.h \
	mettreenlignedialog_editeur.h \
	../Commun/choixniveaudialog_tuxfisher.h \
	../Commun/definitions.h \
	ordrecartedialog.h

CONFIG += release warn_on ressources

RESOURCES += ressources_editeur.qrc

DEPENDPATH += .

SOURCES += main.cpp \
	mainwindow_editeur.cpp \
	widget_editeur.cpp \
	niveaudialog_editeur.cpp \
	../Commun/gestionniveau_tuxfisher.cpp \
	cartedialog_editeur.cpp \
	mettreenlignedialog_editeur.cpp \
	../Commun/choixniveaudialog_tuxfisher.cpp \
	ordrecartedialog.cpp

TEMPLATE = app

TRANSLATIONS = app_en.ts

QT += network

INCLUDEPATH += .

TARGET = Editeur_TuxFisher
