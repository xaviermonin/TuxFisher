NAME = "Editeur de TuxFisher"
LEADER = "Xavier Monin"
LICENSE = "GPLv2"
EMAIL = "tuxfisher@free.fr"
DESCRIPTION = "Editeur de niveaux pour TuxFisher"
HEADERS += ../Commun/definitions.h \
    ../Commun/LevelManager.h \
    ../Commun/ChoiceLevelDialog.h \
    MapDialog.h \
    MainWindow.h \
    LevelUploader.h \
    LevelDialog.h \
    MapSortDialog.h \
    EditorWidget.h
CONFIG += release \
    warn_on \
    ressources
RESOURCES += ressources_editeur.qrc
DEPENDPATH += .
SOURCES += main.cpp \
    ../Commun/LevelManager.cpp \
    ../Commun/ChoiceLevelDialog.cpp \
    MapDialog.cpp \
    MainWindow.cpp \
    LevelUploader.cpp \
    LevelDialog.cpp \
    MapSortDialog.cpp \
    EditorWidget.cpp
TEMPLATE = app
TRANSLATIONS = app_en.ts
QT += network
INCLUDEPATH += .
TARGET = Editeur_TuxFisher
