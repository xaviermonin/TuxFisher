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

#ifndef _GAME_WIDGET_H_
#define _GAME_WIDGET_H_

#include <QGLWidget> //#include <QWidget>//

#include "SoundManager.h"

class QSound;
class QImage;
class GameEngine;

class GameWidget : public QGLWidget // public QWidget//
{
	Q_OBJECT

public:
	GameWidget(QWidget *parent=0);
	~GameWidget();
	inline virtual QSize sizeHint() const { return bufferimage.size(); };
	bool loadMap(int, int, QByteArray);
	void loadGraphism(QString);
	void loadSound(QString);

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void keyPressEvent(QKeyEvent *);
private:
	void generateImage(bool forcer_affichage=false);
	QImage generateWelcomeImage();

	QImage avalanche, fish, tuxfisher, tuxfisher_mort, tuxfisher_vivant, tuxfisher_gagne, neige, montagne, vide;
	sound son_montagne, son_fish, son_mort, son_avalanche, son_victoire;
	GameEngine *moteur;
	SoundManager *audio;

	QImage bufferimage;
	QTimer *timer_animation;
    
	int nbcase_width; // Namebre de Cases (height) dans le widget
	int nbcase_height;
	int case_width; // Namebre de pixel d'une case
	int case_height;

	bool animation_en_cours;
	bool avalancheTombant;
	bool afficher_accueil; // true: Affiche l'accueil (une simple image)
signals:
	void gameOver();
	void levelCompleted();
	void addMove(bool);
private slots:
	void animation();
};

#endif // !_WIDGET_TUXFISHER_H
