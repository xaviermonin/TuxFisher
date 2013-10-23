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

#include <iostream>
#include <QtGui>

#include "GameWidget.h"
#include "GameEngine.h"
#include "SoundManager.h"

GameWidget::GameWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) // QWidget(parent) //
{
	moteur = new GameEngine();
	audio = SoundManager::getInstance();
	son_montagne = son_avalanche = son_mort = son_fish = son_victoire = 0;
	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	animation_en_cours=false;
	avalancheTombant=false;
	afficher_accueil=true;
	timer_animation = new QTimer(this);
	connect(timer_animation, SIGNAL(timeout()), this, SLOT(animation()));
	generateImage();
	setFocus();
}
/*
Plateau::Plateau(QWidget *parent)
: QWidget(parent)
{
	setVisible(false);
	moteur = new MoteurTuxFisher();
	audio = gestionSon_TuxFisher::getInstance();
	son_montagne = son_avalanche = son_mort = son_fish = 0;
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	animation_en_cours=false;
	forcer_affichage=false;
	avalancheTombant=false;
	timer_animation = new QTimer(this);
	connect(timer_animation, SIGNAL(timeout()), this, SLOT(animation()));
}*/

GameWidget::~GameWidget()
{
	delete moteur;
}

void GameWidget::loadGraphism(QString repertoire)
{
	int size;

	if (repertoire == tr("Classique")) // Si graphisme par défaut
		repertoire = ":/ressources/widget/";
	else
		repertoire = QString("graphismes")+QString(QDir::separator())+repertoire+QString(QDir::separator());

	if (!avalanche.load(repertoire+"avalanche.png"))
		std::cout << "Error chargement de avalanche" << std::endl;
	if (!neige.load(repertoire+"neige.png"))
		std::cout << "Error chargement de neige" << std::endl;
	if (!tuxfisher_vivant.load(repertoire+"tuxfisher_vivant.png"))
		std::cout << "Error chargement de TuxFisher :P" << std::endl;
	if (!tuxfisher_gagne.load(repertoire+"tuxfisher_gagne.png"))
		std::cout << "Error chargement de TuxFisher :P" << std::endl;
	if (!tuxfisher_mort.load(repertoire+"tuxfisher_mort.png"))
		std::cout << "Error chargement de TuxFisher :P" << std::endl;
	if (!fish.load(repertoire+"fish.png"))
		std::cout << "Error chargement de fish" << std::endl;
	if (!montagne.load(repertoire+"montagne.png"))
		std::cout << "Error chargement de montagne" << std::endl;
	if (!vide.load(repertoire+"vide.png"))
		std::cout << "Error chargement de vide" << std::endl;

	// Redimensionne correctement les cases (pour les différentes résolutions d'écrans)
	if (QApplication::desktop()->width()>1024 && QApplication::desktop()->height()>768) // ]1024*768;infini[
		size=32;
	else if (QApplication::desktop()->width()>800 && QApplication::desktop()->height()>600) // ]800*600;1024*768]
		size=30;
	else if (QApplication::desktop()->width()>600 && QApplication::desktop()->height()>480) // ]600*480;800*600]
		size=22;
	else
		size=16;
	
	avalanche=avalanche.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	neige=neige.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_vivant=tuxfisher_vivant.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_gagne=tuxfisher_gagne.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_mort=tuxfisher_mort.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	fish=fish.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	montagne=montagne.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	vide=vide.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


	if (moteur->gameOver())
		tuxfisher = tuxfisher_mort;
	else
		tuxfisher = tuxfisher_vivant;

	case_width=avalanche.width();
	case_height=avalanche.height();
	// La size des cases peut avoir set
	if (!afficher_accueil)
		bufferimage = QImage(QSize(nbcase_width*case_width,nbcase_height*case_height),QImage::Format_ARGB32); // Nouvelle size du buffer
	generateImage(true);
}

void GameWidget::loadSound(QString repertoire)
{
	audio->removeSons();
	son_montagne = son_avalanche = son_mort = son_fish = son_victoire = 0;
	if (repertoire == tr("Aucun son"))
		return;

	QString dir_name("sons"+(QString)QDir::separator()+repertoire+(QString)QDir::separator());
	if (!(son_avalanche = audio->loadSound((dir_name+"avalanche.wav").toLatin1().data())))
		std::cout << "Error chargement de " << (dir_name+"avalanche.wav").toLatin1().data() << std::endl;
	if (!(son_mort = audio->loadSound((dir_name+"mort.wav").toLatin1().data())))
		std::cout << "Error chargement de mort.wav" << std::endl;
	if (!(son_fish = audio->loadSound((dir_name+"fish.wav").toLatin1().data())))
		std::cout << "Error chargement de fish.wav" << std::endl;
	if (!(son_montagne = audio->loadSound((dir_name+"montagne.wav").toLatin1().data())))
		std::cout << "Error chargement de montagne.wav" << std::endl;
	if (!(son_victoire = audio->loadSound((dir_name+"victoire.wav").toLatin1().data())))
		std::cout << "Error chargement de victoire.wav" << std::endl;
	std::cerr << audio->retourError().toLatin1().data();
}

bool GameWidget::loadMap(int x, int y, QByteArray datasmap)
{
	tuxfisher = tuxfisher_vivant;
	if (datasmap.size()!=x*y)
		return false;
	
	afficher_accueil=false;
	nbcase_width=x;
	nbcase_height=y;
	bufferimage = QImage(QSize(nbcase_width*case_width,nbcase_height*case_height),QImage::Format_ARGB32);
	moteur->loadMap(nbcase_width, nbcase_height, datasmap.data());
	generateImage(true);
	return true;
}

void GameWidget::animation()
{
	bool tuxfisher_deja_mort=moteur->gameOver();

	animation_en_cours=true;

	if(avalancheTombant)
	{
		if(moteur->dropAvalanche())
			generateImage();
		else
		{
			if (moteur->gameOver()  && !tuxfisher_deja_mort)
			{
				tuxfisher = tuxfisher_mort;
				audio->playSound(son_mort, false);
				generateImage(true);
			}
			avalancheTombant=false;
			audio->playSound(son_avalanche, true);
		}
		return;
	}

	if(moteur->fallingAvalanche())
		avalancheTombant=true;
	else
	{
		// Animation terminée
		avalancheTombant=false;
		animation_en_cours=false;
		timer_animation->stop();
		if (moteur->gameOver()) // On signale le gameover si tuxfisher est mort et que l'animation est terminée
			emit gameOver();
	}
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
	if (animation_en_cours || moteur->gameOver() || moteur->levelCompleted())
	{
		//event->ignore();
		QWidget::keyPressEvent(event);
		return;
	}
	RETOURMVT retourmvt=RET_MONTAGNE;
	switch (event->key())
	{
		case Qt::Key_Left:
			retourmvt=moteur->moveTuxFisher(MVT_GAUCHE);
			break;
		case Qt::Key_Right:
			retourmvt=moteur->moveTuxFisher(MVT_DROITE);
			break;
		case Qt::Key_Down:
			retourmvt=moteur->moveTuxFisher(MVT_BAS);
			break;
		case Qt::Key_Up:
			retourmvt=moteur->moveTuxFisher(MVT_HAUT);
			break;
		case Qt::Key_Backspace:
			retourmvt = moteur->getBackToPrevMap();
			break;
		default:
			QWidget::keyPressEvent(event);
			//event->ignore();
			return;
	}
	
	switch (retourmvt)
	{
		case RET_POISSON:
			timer_animation->start(0);
			emit addMove(true);
			audio->playSound(son_fish, false);
			break;
		case RET_MONTAGNE:
		case RET_AVALANCHE:
		case RET_RIEN:
			audio->playSound(son_montagne, false);
			break;
		case RET_RETOUR:
			emit addMove(false);
			break;
		case RET_NEIGE:
		case RET_VIDE:
			timer_animation->start(0);
			emit addMove(true);
			break;
		default:
			break;
	}
	if (moteur->levelCompleted())
	{
		tuxfisher = tuxfisher_gagne;
		audio->playSound(son_victoire);
		emit levelCompleted();
	}

	generateImage();
}

void GameWidget::paintEvent(QPaintEvent *)
{
	// Calcul du point pour centrer l'affichage par rapport a la size du Widget
	QPoint centre=rect().center();
	centre -= QPoint(sizeHint().width(), sizeHint().height())/2;

 	QPainter painter_widget(this);
 	
	painter_widget.drawImage(centre, bufferimage); // On affiche à l'écran le buffer
	painter_widget.end();
}

void GameWidget::generateImage(bool forcer_affichage) // Génére l'image à afficher
{
	if (afficher_accueil)
	{
		bufferimage = generateWelcomeImage();
		updateGeometry();
		update();
		return;
	}

	QPainter painter_buffer(&bufferimage);
 	 	
	bool affichage;
	
	for (int county=0 ; county<nbcase_height ; county++)
	{
		for(int countx=0 ; countx<nbcase_width ; countx++)
		{
			// Si on peut optimiser l'affichage et qu'on ne doit pas forcer l'affichage)
			if (moteur->getCaseMapOptimisation(0, 0)!=ERREUR && !forcer_affichage)
			{
				if (moteur->getCase(countx, county)!=moteur->getCaseMapOptimisation(countx, county))
					affichage=true;
				else
					affichage=false;
			} // Sinon on affiche toute la map
			else
				affichage=true;
			
			// On affiche que ce qui doit être affiché
			if (affichage)
				switch(moteur->getCase(countx, county))
				{
					case CASE_VIDE:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),vide); break;
					case CASE_AVALANCHE:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),avalanche); break;
					case CASE_TUXFISHER:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),tuxfisher); break;
					case CASE_POISSON:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),fish); break;
					case CASE_MONTAGNE:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),montagne); break;
					case CASE_NEIGE:
						painter_buffer.drawImage(QPoint(countx*case_width,county*case_height),neige); break;
					case ERREUR:
						std::cout << "ERREUR"; break;
				}
		}
	}
	painter_buffer.end(); //Fin du buffer
	moteur->loadMapOptimisation(); // On enregistre la map pour les prochaines modifications partiels
	if (forcer_affichage)
		updateGeometry();
	update();
}

QImage GameWidget::generateWelcomeImage()
{
	QImage image_accueil=QImage(":/ressources/mainwindow/fond.png");
	int height;
	if (QApplication::desktop()->width()<1024 && QApplication::desktop()->height()<768) // ]-infinie;1024*768[
	{
		if (QApplication::desktop()->width()>=800 && QApplication::desktop()->height()>=600) // [800*600;1024*768[
			height=400;
		else if (QApplication::desktop()->width()>=600 && QApplication::desktop()->height()>=480) // [600*480;800*600[
			height=300;
		else // ]-infinie;600*480[
			height=100;
		image_accueil=image_accueil.scaled(height, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	}
	return image_accueil;
}
