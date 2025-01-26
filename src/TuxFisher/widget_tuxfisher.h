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

#ifndef _WIDGET_TUXFISHER_H_
#define _WIDGET_TUXFISHER_H_

#include <QGLWidget> //#include <QWidget>//

#include "gestionson_tuxfisher.h"

class QSound;
class QImage;
class MoteurTuxFisher;


class Widget_TuxFisher : public QGLWidget // public QWidget// 
{
	Q_OBJECT

public:
	Widget_TuxFisher(QWidget *parent=0);
	~Widget_TuxFisher();
	inline virtual QSize sizeHint() const { return bufferimage.size(); };
	bool chargerMap(int, int, QByteArray);
	void chargerGraphisme(QString);
	void chargerSon(QString);

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void keyPressEvent(QKeyEvent *);
private:
	void genererImage(bool forcer_affichage=false);
	QImage genererImageAccueil();

	QImage avalanche, poisson, tuxfisher, tuxfisher_mort, tuxfisher_vivant, tuxfisher_gagne, neige, montagne, vide;
	son son_montagne, son_poisson, son_mort, son_avalanche, son_victoire;
	MoteurTuxFisher *moteur;
	gestionSon_TuxFisher *audio;

    QImage bufferimage;
    QTimer *timer_animation;
    
	int nbcase_longueur; // Nombre de Cases (hauteur) dans le widget
	int nbcase_hauteur;
	int case_longueur; // Nombre de pixel d'une case
    int case_hauteur;

    bool animation_en_cours;
	bool avalancheTombant;
	bool afficher_accueil; // true: Affiche l'accueil (une simple image)
signals:
	void gameOver();
	void niveauTermine();
	void ajouterMouvement(bool);
private slots:
	void animation();
};

#endif // !_WIDGET_TUXFISHER_H
