/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de levelx pour TuxFisher
Copyright 2008 - Xavier Monin - Tous droits réservés
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	
	// Chargement des traductions
	QTranslator appTranslator;
	appTranslator.load("app_"+QLocale::system().name(), ":/ressources/traductions");
	app.installTranslator(&appTranslator);
	
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), ":/ressources/traductions");
	app.installTranslator(&qtTranslator);

	// Création de la fenêtre principale
	MainWindow mw_editor;
	mw_editor.show();
	
	return app.exec();
}
