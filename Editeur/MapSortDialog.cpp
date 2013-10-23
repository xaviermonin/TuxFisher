/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade 2D mettant en scène le personnage Tux
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

#include <QtGui>
#include <iostream>
#include "MapSortDialog.h"

MapSortDialog::MapSortDialog(QWidget *parent)
: QDialog(parent)
{
	vue = new QListView(this);
	modele = new QStringListModel(this);
	vue->setModel(modele);
	button_ok = new QPushButton(tr("OK"),this);
	button_up = new QPushButton(tr("Haut"),this);
	button_down = new QPushButton(tr("Bas"),this);

	QVBoxLayout *vertical_layout = new QVBoxLayout; // Boutons Haut et Bas
	QHBoxLayout *horizontal_layout = new QHBoxLayout; // La vue et les buttons up et down
	QHBoxLayout *horizontal_buttonok_layout = new QHBoxLayout; // Le button ok et les spacers pour centrer
	QVBoxLayout *layout = new QVBoxLayout;

	vertical_layout->addWidget(button_up);
	vertical_layout->addWidget(button_down);
	horizontal_layout->addWidget(vue);
	horizontal_buttonok_layout->addStretch();
	horizontal_buttonok_layout->addWidget(button_ok);
	horizontal_buttonok_layout->addStretch();
	horizontal_layout->addLayout(vertical_layout);
	layout->addLayout(horizontal_layout);
	layout->addLayout(horizontal_buttonok_layout);
	setLayout(layout);

	connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(button_up, SIGNAL(clicked()), this, SLOT(up()));
	connect(button_down, SIGNAL(clicked()), this, SLOT(down()));
}

MapSortDialog::~MapSortDialog()
{
	
}

void MapSortDialog::loadStringList(QStringList list)
{
	list_string=list;
	modele->setStringList(list_string);
	for (int count=0; count<list.count(); count++)
		sort[count]=count;
}

void MapSortDialog::up()
{
	QModelIndex index=vue->currentIndex();
	if (!index.isValid())
		return;
	int position=index.row();
	if (position>0)
	{
		QString string_temp=list_string.at(position);
		list_string[position]=list_string.at(position-1);
		list_string[position-1]=string_temp;

		int int_temp=sort[position];
		sort[position]=sort[position-1];
		sort[position-1]=int_temp;

		modele->setStringList(list_string);
		vue->setCurrentIndex(modele->index(position-1, 0)); // Sélectionne l'objet qui est monté
	}
}

void MapSortDialog::down()
{
	QModelIndex index=vue->currentIndex();
	if (!index.isValid())
		return;
	int position=index.row();
	if (position<(list_string.count()-1))
	{
		QString string_temp=list_string.at(position);
		list_string[position]=list_string.at(position+1);
		list_string[position+1]=string_temp;

		int int_temp=sort[position];
		sort[position]=sort[position+1];
		sort[position+1]=int_temp;

		modele->setStringList(list_string);
		vue->setCurrentIndex(modele->index(position+1, 0)); // Sélectionne l'objet qui est descendu
	}
}
