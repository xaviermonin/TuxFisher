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

#include <iostream>
#include <QtGui>
#include "widget_editeur.h"

Widget_Editeur::Widget_Editeur(QWidget *parent) : QWidget(parent)
{
	chargerGraphisme();
	mapcharge=false;
	hauteur=0;
	longueur=0;
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

Widget_Editeur::~Widget_Editeur()
{
	if (mapcharge)
	{
		for(int i=0 ; i<hauteur ; i++)
        	delete map[i];
	    delete [] map;
	}
}

void Widget_Editeur::chargerGraphisme()
{
	if (!avalanche.load(":/ressources/widget/avalanche.png"))
		std::cout << "Erreur chargement de avalanche" << std::endl;
	if (!neige.load(":/ressources/widget/neige.png"))
		std::cout << "Erreur chargement de neige" << std::endl;
	if (!tuxfisher.load(":/ressources/widget/tuxfisher.png"))
		std::cout << "Erreur chargement de TuxFisher :P" << std::endl;
	if (!poisson.load(":/ressources/widget/poisson.png"))
		std::cout << "Erreur chargement de poisson" << std::endl;
	if (!montagne.load(":/ressources/widget/montagne.png"))
		std::cout << "Erreur chargement de montagne" << std::endl;
	if (!vide.load(":/ressources/widget/vide.png"))
		std::cout << "Erreur chargement de vide" << std::endl;
}

bool Widget_Editeur::nouvelleCarte(int x, int y)
{
	if (mapcharge)
	{
		for(int i=0 ; i<hauteur ; i++)
        	delete map[i];
	    delete [] map;
	}
	longueur=x;
	hauteur=y;
	map = new CASE*[longueur];
    for(int i=0 ; i<longueur ; i++)
        map[i] = new CASE[hauteur];
	for (int countx=0 ; countx < longueur ; countx++)
        for (int county=0 ; county < hauteur ; county++)
           	map[countx][county] = CASE_NEIGE;
	mapcharge=true;
	return true;
}

bool Widget_Editeur::chargerCarte(int x, int y, QByteArray donnesmap)
{
	if (x<2 || y<2)
		return false;
	if (donnesmap.size()!=x*y)
		return false;
	if (mapcharge)
	{
		for(int i=0 ; i<hauteur ; i++)
        	delete map[i];
	    delete [] map;
	}
	longueur=x;
	hauteur=y;
	int count=0;
	map = new CASE*[longueur];
    for(int i=0 ; i<longueur ; i++)
        map[i] = new CASE[longueur];
	for (int countx=0 ; countx < longueur ; countx++)
        for (int county=0 ; county < hauteur ; county++)
        {
         	map[countx][county] = (CASE)(donnesmap.data()[count]);
         	count++;
        }
	mapcharge=true;
	return true;
}

QByteArray Widget_Editeur::recupererCarte()
{
	if (!mapcharge)
		return "";
	QByteArray retour;
	for (int countx=0; countx<longueur ; countx++)
		for (int county=0 ; county<hauteur ; county++)
			retour+=map[countx][county];
	return retour;
}

void Widget_Editeur::selectionnerPinceau(CASE selection)
{
	pinceau=selection;
}

bool Widget_Editeur::tuxfisherPresent()
{
	int countxydion=0;
	for (int countx=0; countx<longueur ; countx++)
		for (int county=0 ; county<hauteur ; county++)
			if (map[countx][county] == CASE_TUXFISHER)
			countxydion++;
	if (countxydion==1)
		return true;
	else
		return false;
}

QSize Widget_Editeur::sizeHint() const
{
	QSize size;
	size.setHeight(avalanche.height()*hauteur);
	size.setWidth(avalanche.width()*longueur);
	return size;
}

void Widget_Editeur::mouseMoveEvent(QMouseEvent *event)
{
	if (mapcharge)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			int pos_x = event->x()/32;
			int pos_y = event->y()/32;
			if (pos_x < longueur && pos_y < hauteur && pos_x>=0 && pos_y>=0)
				if (map[pos_x][pos_y]!=pinceau)
				{
					map[pos_x][pos_y]=pinceau;
					emit modifications();
					update();
				}
		}
	}
}

void Widget_Editeur::mousePressEvent(QMouseEvent *event)
{
	if (mapcharge)
	{
		int pos_x = event->x()/32;
		int pos_y = event->y()/32;
		if (pos_x < longueur && pos_y < hauteur && pos_x>=0 && pos_y>=0)
			if (map[pos_x][pos_y]!=pinceau)
			{
				map[pos_x][pos_y]=pinceau;
				emit modifications();
				update();
			}

	}
}

void Widget_Editeur::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    // La taille de l'image avalanche est définie comme taille par défaut
    int image_longueur=avalanche.width();
    int image_hauteur=avalanche.height();
    
	for (int county=0 ; county<hauteur ; county++)
	{
		for(int countx=0 ; countx<longueur ; countx++)
		{
			switch(map[countx][county])
			{
				case CASE_VIDE:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),vide);
					break;
				case CASE_AVALANCHE:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),avalanche);
					break;
				case CASE_TUXFISHER:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),tuxfisher);
					break;
				case CASE_POISSON:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),poisson);
					break;
				case CASE_MONTAGNE:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),montagne);
					break;
				case CASE_NEIGE:
					painter.drawImage(QPoint(countx*image_longueur,county*image_hauteur),neige);
					break;
				case ERREUR:
					break;
			}
		}
	}
}
