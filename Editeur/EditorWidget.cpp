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

#include <iostream>
#include <QtGui>
#include "EditorWidget.h"

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent)
{
	loadGraphism();
	map_loaded=false;
	height=0;
	width=0;
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

EditorWidget::~EditorWidget()
{
	if (map_loaded)
	{
		for(int i=0 ; i<height ; ++i)
        	delete map[i];
	    delete [] map;
	}
}

void EditorWidget::loadGraphism()
{
	if (!avalanche.load(":/ressources/widget/avalanche.png"))
		std::cout << "Error chargement de avalanche" << std::endl;
	if (!neige.load(":/ressources/widget/neige.png"))
		std::cout << "Error chargement de neige" << std::endl;
	if (!tuxfisher.load(":/ressources/widget/tuxfisher.png"))
		std::cout << "Error chargement de TuxFisher :P" << std::endl;
	if (!fish.load(":/ressources/widget/fish.png"))
		std::cout << "Error chargement de fish" << std::endl;
	if (!montagne.load(":/ressources/widget/montagne.png"))
		std::cout << "Error chargement de montagne" << std::endl;
	if (!vide.load(":/ressources/widget/vide.png"))
		std::cout << "Error chargement de vide" << std::endl;
}

bool EditorWidget::newMap(int x, int y)
{
	if (map_loaded)
	{
		for(int i=0 ; i<height ; ++i)
        	delete map[i];
	    delete [] map;
	}
	width=x;
	height=y;
	map = new CASE*[width];
    for(int i=0 ; i<width ; ++i)
        map[i] = new CASE[height];
	for (int countx=0 ; countx < width ; countx++)
        for (int county=0 ; county < height ; county++)
           	map[countx][county] = CASE_NEIGE;
	map_loaded=true;
	return true;
}

bool EditorWidget::loadMap(int x, int y, QByteArray datasmap)
{
	if (x<2 || y<2)
		return false;
	if (datasmap.size()!=x*y)
		return false;
	if (map_loaded)
	{
		for(int i=0 ; i<height ; ++i)
        	delete map[i];
	    delete [] map;
	}
	width=x;
	height=y;
	int count=0;
	map = new CASE*[width];
    for(int i=0 ; i<width ; ++i)
        map[i] = new CASE[width];
	for (int countx=0 ; countx < width ; countx++)
        for (int county=0 ; county < height ; county++)
        {
         	map[countx][county] = (CASE)(datasmap.data()[count]);
         	count++;
        }
	map_loaded=true;
	return true;
}

QByteArray EditorWidget::getMap()
{
	if (!map_loaded)
		return "";
	QByteArray retour;
	for (int countx=0; countx<width ; countx++)
		for (int county=0 ; county<height ; county++)
			retour+=map[countx][county];
	return retour;
}

void EditorWidget::selectBrush(CASE selection)
{
	brush=selection;
}

bool EditorWidget::isTuxPresent()
{
	int countxydion=0;
	for (int countx=0; countx<width ; countx++)
		for (int county=0 ; county<height ; county++)
			if (map[countx][county] == CASE_TUXFISHER)
			countxydion++;
	if (countxydion==1)
		return true;
	else
		return false;
}

QSize EditorWidget::sizeHint() const
{
	QSize size;
	size.setHeight(avalanche.height()*height);
	size.setWidth(avalanche.width()*width);
	return size;
}

void EditorWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (map_loaded)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			int pos_x = event->x()/32;
			int pos_y = event->y()/32;
			if (pos_x < width && pos_y < height && pos_x>=0 && pos_y>=0)
				if (map[pos_x][pos_y]!=brush)
				{
					map[pos_x][pos_y]=brush;
					emit modifications();
					update();
				}
		}
	}
}

void EditorWidget::mousePressEvent(QMouseEvent *event)
{
	if (map_loaded)
	{
		int pos_x = event->x()/32;
		int pos_y = event->y()/32;
		if (pos_x < width && pos_y < height && pos_x>=0 && pos_y>=0)
			if (map[pos_x][pos_y]!=brush)
			{
				map[pos_x][pos_y]=brush;
				emit modifications();
				update();
			}

	}
}

void EditorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    // La size de l'image avalanche est définie comme size par défaut
    int image_width=avalanche.width();
    int image_height=avalanche.height();
    
	for (int county=0 ; county<height ; county++)
	{
		for(int countx=0 ; countx<width ; countx++)
		{
			switch(map[countx][county])
			{
				case CASE_VIDE:
					painter.drawImage(QPoint(countx*image_width,county*image_height),vide);
					break;
				case CASE_AVALANCHE:
					painter.drawImage(QPoint(countx*image_width,county*image_height),avalanche);
					break;
				case CASE_TUXFISHER:
					painter.drawImage(QPoint(countx*image_width,county*image_height),tuxfisher);
					break;
				case CASE_POISSON:
					painter.drawImage(QPoint(countx*image_width,county*image_height),fish);
					break;
				case CASE_MONTAGNE:
					painter.drawImage(QPoint(countx*image_width,county*image_height),montagne);
					break;
				case CASE_NEIGE:
					painter.drawImage(QPoint(countx*image_width,county*image_height),neige);
					break;
				case ERREUR:
					break;
			}
		}
	}
}
