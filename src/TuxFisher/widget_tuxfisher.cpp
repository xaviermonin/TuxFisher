/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 03/06/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en scène le personnage Tux
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

#include <iostream>
#include <QtGui>

#include "widget_tuxfisher.h"
#include "moteur_tuxfisher.h"
#include "gestionson_tuxfisher.h"

Widget_TuxFisher::Widget_TuxFisher(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) // QWidget(parent) //
{
	moteur = new MoteurTuxFisher();
	audio = gestionSon_TuxFisher::getInstance();
	son_montagne = son_avalanche = son_mort = son_poisson = son_victoire = 0;
	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	animation_en_cours=false;
	avalancheTombant=false;
	afficher_accueil=true;
	timer_animation = new QTimer(this);
	connect(timer_animation, SIGNAL(timeout()), this, SLOT(animation()));
	genererImage();
	setFocus();
}
/*
Widget_TuxFisher::Widget_TuxFisher(QWidget *parent)
: QWidget(parent)
{
	setVisible(false);
	moteur = new MoteurTuxFisher();
	audio = gestionSon_TuxFisher::getInstance();
	son_montagne = son_avalanche = son_mort = son_poisson = 0;
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	animation_en_cours=false;
	forcer_affichage=false;
	avalancheTombant=false;
	timer_animation = new QTimer(this);
	connect(timer_animation, SIGNAL(timeout()), this, SLOT(animation()));
}*/

Widget_TuxFisher::~Widget_TuxFisher()
{
	delete moteur;
}

void Widget_TuxFisher::chargerGraphisme(QString repertoire)
{
	int taille;

	if (repertoire == tr("Classique")) // Si graphisme par défaut
		repertoire = ":/ressources/widget/";
	else
		repertoire = QString("graphismes")+QString(QDir::separator())+repertoire+QString(QDir::separator());

	if (!avalanche.load(repertoire+"avalanche.png"))
		std::cout << "Erreur chargement de avalanche" << std::endl;
	if (!neige.load(repertoire+"neige.png"))
		std::cout << "Erreur chargement de neige" << std::endl;
	if (!tuxfisher_vivant.load(repertoire+"tuxfisher_vivant.png"))
		std::cout << "Erreur chargement de TuxFisher :P" << std::endl;
	if (!tuxfisher_gagne.load(repertoire+"tuxfisher_gagne.png"))
		std::cout << "Erreur chargement de TuxFisher :P" << std::endl;
	if (!tuxfisher_mort.load(repertoire+"tuxfisher_mort.png"))
		std::cout << "Erreur chargement de TuxFisher :P" << std::endl;
	if (!poisson.load(repertoire+"poisson.png"))
		std::cout << "Erreur chargement de poisson" << std::endl;
	if (!montagne.load(repertoire+"montagne.png"))
		std::cout << "Erreur chargement de montagne" << std::endl;
	if (!vide.load(repertoire+"vide.png"))
		std::cout << "Erreur chargement de vide" << std::endl;

	// Redimensionne correctement les cases (pour les différentes résolutions d'écrans)
	if (QApplication::desktop()->width()>1024 && QApplication::desktop()->height()>768) // ]1024*768;infini[
		taille=32;
	else if (QApplication::desktop()->width()>800 && QApplication::desktop()->height()>600) // ]800*600;1024*768]
		taille=30;
	else if (QApplication::desktop()->width()>600 && QApplication::desktop()->height()>480) // ]600*480;800*600]
		taille=22;
	else
		taille=16;
	
	avalanche=avalanche.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	neige=neige.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_vivant=tuxfisher_vivant.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_gagne=tuxfisher_gagne.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	tuxfisher_mort=tuxfisher_mort.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	poisson=poisson.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	montagne=montagne.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	vide=vide.scaled(taille, taille, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


	if (moteur->gameOver())
		tuxfisher = tuxfisher_mort;
	else
		tuxfisher = tuxfisher_vivant;

	case_longueur=avalanche.width();
	case_hauteur=avalanche.height();
	// La taille des cases peut avoir changer
	if (!afficher_accueil)
		bufferimage = QImage(QSize(nbcase_longueur*case_longueur,nbcase_hauteur*case_hauteur),QImage::Format_ARGB32); // Nouvelle taille du buffer
	genererImage(true);
}

void Widget_TuxFisher::chargerSon(QString repertoire)
{
	audio->supprimerSons();
	son_montagne = son_avalanche = son_mort = son_poisson = son_victoire = 0;
	if (repertoire == tr("Aucun son"))
		return;

	QString nomrepertoire("sons"+(QString)QDir::separator()+repertoire+(QString)QDir::separator());
	if (!(son_avalanche = audio->chargerSon((nomrepertoire+"avalanche.wav").toLatin1().data())))
		std::cout << "Erreur chargement de " << (nomrepertoire+"avalanche.wav").toLatin1().data() << std::endl;
	if (!(son_mort = audio->chargerSon((nomrepertoire+"mort.wav").toLatin1().data())))
		std::cout << "Erreur chargement de mort.wav" << std::endl;
	if (!(son_poisson = audio->chargerSon((nomrepertoire+"poisson.wav").toLatin1().data())))
		std::cout << "Erreur chargement de poisson.wav" << std::endl;
	if (!(son_montagne = audio->chargerSon((nomrepertoire+"montagne.wav").toLatin1().data())))
		std::cout << "Erreur chargement de montagne.wav" << std::endl;
	if (!(son_victoire = audio->chargerSon((nomrepertoire+"victoire.wav").toLatin1().data())))
		std::cout << "Erreur chargement de victoire.wav" << std::endl;
	std::cerr << audio->retourErreur().toLatin1().data();
}

bool Widget_TuxFisher::chargerMap(int x, int y, QByteArray donnesmap)
{
	tuxfisher = tuxfisher_vivant;
	if (donnesmap.size()!=x*y)
		return false;
	
	afficher_accueil=false;
	nbcase_longueur=x;
	nbcase_hauteur=y;
	bufferimage = QImage(QSize(nbcase_longueur*case_longueur,nbcase_hauteur*case_hauteur),QImage::Format_ARGB32);
	moteur->chargerMap(nbcase_longueur, nbcase_hauteur, donnesmap.data());
	genererImage(true);
	return true;
}

void Widget_TuxFisher::animation()
{
	bool tuxfisher_deja_mort=moteur->gameOver();

	animation_en_cours=true;

	if(avalancheTombant)
	{
		if(moteur->faireTomberAvalanche())
			genererImage();
		else
		{
			if (moteur->gameOver()  && !tuxfisher_deja_mort)
			{
				tuxfisher = tuxfisher_mort;
				audio->jouerSon(son_mort, false);
				genererImage(true);
			}
			avalancheTombant=false;
			audio->jouerSon(son_avalanche, true);
		}
		return;
	}

	if(moteur->avalancheDevantTomber())
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

void Widget_TuxFisher::keyPressEvent(QKeyEvent *event)
{
	if (animation_en_cours || moteur->gameOver() || moteur->niveauTermine())
	{
		//event->ignore();
		QWidget::keyPressEvent(event);
		return;
	}
	RETOURMVT retourmvt=RET_MONTAGNE;
	switch (event->key())
	{
		case Qt::Key_Left:
			retourmvt=moteur->mouvementTuxFisher(MVT_GAUCHE);
			break;
		case Qt::Key_Right:
			retourmvt=moteur->mouvementTuxFisher(MVT_DROITE);
			break;
		case Qt::Key_Down:
			retourmvt=moteur->mouvementTuxFisher(MVT_BAS);
			break;
		case Qt::Key_Up:
			retourmvt=moteur->mouvementTuxFisher(MVT_HAUT);
			break;
		case Qt::Key_Backspace:
			retourmvt = moteur->revenirCartePrecedente();
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
			emit ajouterMouvement(true);
			audio->jouerSon(son_poisson, false);
			break;
		case RET_MONTAGNE:
		case RET_AVALANCHE:
		case RET_RIEN:
			audio->jouerSon(son_montagne, false);
			break;
		case RET_RETOUR:
			emit ajouterMouvement(false);
			break;
		case RET_NEIGE:
		case RET_VIDE:
			timer_animation->start(0);
			emit ajouterMouvement(true);
			break;
		default:
			break;
	}
	if (moteur->niveauTermine())
	{
		tuxfisher = tuxfisher_gagne;
		audio->jouerSon(son_victoire);
		emit niveauTermine();
	}

	genererImage();
}

void Widget_TuxFisher::paintEvent(QPaintEvent *)
{
	// Calcul du point pour centrer l'affichage par rapport a la taille du Widget
	QPoint centre=rect().center();
	centre -= QPoint(sizeHint().width(), sizeHint().height())/2;

 	QPainter painter_widget(this);
 	
	painter_widget.drawImage(centre, bufferimage); // On affiche à l'écran le buffer
	painter_widget.end();
}

void Widget_TuxFisher::genererImage(bool forcer_affichage) // Génére l'image à afficher
{
	if (afficher_accueil)
	{
		bufferimage = genererImageAccueil();
		updateGeometry();
		update();
		return;
	}

	QPainter painter_buffer(&bufferimage);
 	 	
	bool affichage;
	
	for (int county=0 ; county<nbcase_hauteur ; county++)
	{
		for(int countx=0 ; countx<nbcase_longueur ; countx++)
		{
			// Si on peut optimiser l'affichage et qu'on ne doit pas forcer l'affichage)
			if (moteur->recupererCaseCarteOptimisation(0, 0)!=ERREUR && !forcer_affichage)
			{
				if (moteur->recupererCase(countx, county)!=moteur->recupererCaseCarteOptimisation(countx, county))
					affichage=true;
				else
					affichage=false;
			} // Sinon on affiche toute la carte
			else
				affichage=true;
			
			// On affiche que ce qui doit être affiché
			if (affichage)
				switch(moteur->recupererCase(countx, county))
				{
					case CASE_VIDE:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),vide); break;
					case CASE_AVALANCHE:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),avalanche); break;
					case CASE_TUXFISHER:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),tuxfisher); break;
					case CASE_POISSON:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),poisson); break;
					case CASE_MONTAGNE:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),montagne); break;
					case CASE_NEIGE:
						painter_buffer.drawImage(QPoint(countx*case_longueur,county*case_hauteur),neige); break;
					case ERREUR:
						std::cout << "ERREUR"; break;
				}
		}
	}
	painter_buffer.end(); //Fin du buffer
	moteur->chargerCarteOptimisation(); // On enregistre la carte pour les prochaines modifications partiels
	if (forcer_affichage)
		updateGeometry();
	update();
}

QImage Widget_TuxFisher::genererImageAccueil()
{
	QImage image_accueil=QImage(":/ressources/mainwindow/fond.png");
	int hauteur;
	if (QApplication::desktop()->width()<1024 && QApplication::desktop()->height()<768) // ]-infinie;1024*768[
	{
		if (QApplication::desktop()->width()>=800 && QApplication::desktop()->height()>=600) // [800*600;1024*768[
			hauteur=400;
		else if (QApplication::desktop()->width()>=600 && QApplication::desktop()->height()>=480) // [600*480;800*600[
			hauteur=300;
		else // ]-infinie;600*480[
			hauteur=100;
		image_accueil=image_accueil.scaled(hauteur, hauteur, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	}
	return image_accueil;
}
