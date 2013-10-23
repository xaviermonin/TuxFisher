/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 03/06/2010
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en scène le personnage Tux
Copyright 2010 - Xavier Monin - Tous droits réservés
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
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QTranslator>
#include <QLocale>
#include "MainWindow.h"
#include "../Commun/definitions.h"

int main(int argc, char **argv)
{
	bool afficher_splash=false;
	QApplication app(argc, argv);
	QSplashScreen *splash = new QSplashScreen;

	if (QApplication::desktop()->width()>600 && QApplication::desktop()->height()>480) // ]600*480;infinie[
	{
		// Création du SplashScreen de chargement
		int height;
		afficher_splash=true;
		QPixmap pixmap_splash=QPixmap(":/ressources/mainwindow/fond.png");

		if (QApplication::desktop()->width()<1024 && QApplication::desktop()->height()<768) // ]600*480;1024*768[
		{
			// Dans cette partie on rétrécie l'image
			if (QApplication::desktop()->width()>=800 && QApplication::desktop()->height()>=600) // [800*600;1024*768[
				height=400;
			else // ]600*480;800*600[
				height=200;
			pixmap_splash=pixmap_splash.scaled(height, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
		}

		splash->setPixmap(pixmap_splash);
		splash->show();
		splash->showMessage(QObject::tr("Chargement de %1").arg(APPLICATION_NAME), Qt::AlignRight|Qt::AlignTop, Qt::black);
	} // else: pas de splash !
	
	
	// Chargement des traductions
	QTranslator appTranslator;
	appTranslator.load("app_"+QLocale::system().name(), ":/ressources/traductions");
	app.installTranslator(&appTranslator);
	
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), ":/ressources/traductions");
	app.installTranslator(&qtTranslator);
	
	// Création de la fenêtre principale
	MainWindow mainwindow;
	mainwindow.show();
	
	if (afficher_splash)
		splash->finish(&mainwindow);
	delete splash;

	return app.exec();
}
