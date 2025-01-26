/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de niveaux pour TuxFisher
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

#ifndef _WIDGET_EDITEUR_H_
#define _WIDGET_EDITEUR_H_

#include <QWidget>

class QImage;

typedef enum { CASE_AVALANCHE=33, CASE_VIDE, CASE_TUXFISHER, CASE_NEIGE,
			   CASE_POISSON, CASE_MONTAGNE, ERREUR=-1} CASE;

class Widget_Editeur : public QWidget
{
	Q_OBJECT
public:
	Widget_Editeur(QWidget *parent=0);
	~Widget_Editeur();
	void chargerGraphisme();
	QSize sizeHint() const;
	bool nouvelleCarte(int, int);
	bool chargerCarte(int, int, QByteArray);
	bool tuxfisherPresent();
	QByteArray recupererCarte();
public slots:
	void selectionnerPinceau(CASE);
protected:
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
private:
	QImage avalanche, poisson, tuxfisher, neige, montagne, vide;
	int hauteur;
	int longueur;
	CASE pinceau;
	CASE **map;
	bool mapcharge;
signals:
	void modifications();
};

#endif // !_WIDGET_EDITEUR_H_
