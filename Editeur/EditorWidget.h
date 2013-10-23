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

#ifndef _EDITOR_WIDGET_H_
#define _EDITOR_WIDGET_H_

#include <QWidget>

class QImage;

typedef enum { CASE_AVALANCHE=33, CASE_VIDE, CASE_TUXFISHER, CASE_NEIGE,
			   CASE_POISSON, CASE_MONTAGNE, ERREUR=-1} CASE;

class EditorWidget : public QWidget
{
	Q_OBJECT
public:
	EditorWidget(QWidget *parent=0);
	~EditorWidget();
	void loadGraphism();
	QSize sizeHint() const;
	bool newMap(int, int);
	bool loadMap(int, int, QByteArray);
	bool isTuxPresent();
	QByteArray getMap();
public slots:
	void selectBrush(CASE);
protected:
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
private:
	QImage avalanche, fish, tuxfisher, neige, montagne, vide;
	int height;
	int width;
	CASE brush;
	CASE **map;
	bool map_loaded;
signals:
	void modifications();
};

#endif // !_WIDGET_EDITEUR_H_
