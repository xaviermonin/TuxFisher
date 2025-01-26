/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
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
#include "ordrecartedialog.h"

ordreCarteDialog::ordreCarteDialog(QWidget *parent)
: QDialog(parent)
{
	vue = new QListView(this);
	modele = new QStringListModel(this);
	vue->setModel(modele);
	bouton_ok = new QPushButton(tr("OK"),this);
	bouton_haut = new QPushButton(tr("Haut"),this);
	bouton_bas = new QPushButton(tr("Bas"),this);

	QVBoxLayout *vertical_layout = new QVBoxLayout; // Boutons Haut et Bas
	QHBoxLayout *horizontal_layout = new QHBoxLayout; // La vue et les boutons haut et bas
	QHBoxLayout *horizontal_boutonok_layout = new QHBoxLayout; // Le bouton ok et les spacers pour centrer
	QVBoxLayout *layout = new QVBoxLayout;

	vertical_layout->addWidget(bouton_haut);
	vertical_layout->addWidget(bouton_bas);
	horizontal_layout->addWidget(vue);
	horizontal_boutonok_layout->addStretch();
	horizontal_boutonok_layout->addWidget(bouton_ok);
	horizontal_boutonok_layout->addStretch();
	horizontal_layout->addLayout(vertical_layout);
	layout->addLayout(horizontal_layout);
	layout->addLayout(horizontal_boutonok_layout);
	setLayout(layout);

	connect(bouton_ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(bouton_haut, SIGNAL(clicked()), this, SLOT(monterOrdre()));
	connect(bouton_bas, SIGNAL(clicked()), this, SLOT(descendreOrdre()));
}

ordreCarteDialog::~ordreCarteDialog()
{
	
}

void ordreCarteDialog::chargerStringList(QStringList liste)
{
	liste_string=liste;
	modele->setStringList(liste_string);
	for (int count=0; count<liste.count(); count++)
		classement[count]=count;
}

void ordreCarteDialog::monterOrdre()
{
	QModelIndex index=vue->currentIndex();
	if (!index.isValid())
		return;
	int position=index.row();
	if (position>0)
	{
		QString string_temp=liste_string.at(position);
		liste_string[position]=liste_string.at(position-1);
		liste_string[position-1]=string_temp;

		int int_temp=classement[position];
		classement[position]=classement[position-1];
		classement[position-1]=int_temp;

		modele->setStringList(liste_string);
		vue->setCurrentIndex(modele->index(position-1, 0)); // Sélectionne l'objet qui est monté
	}
}

void ordreCarteDialog::descendreOrdre()
{
	QModelIndex index=vue->currentIndex();
	if (!index.isValid())
		return;
	int position=index.row();
	if (position<(liste_string.count()-1))
	{
		QString string_temp=liste_string.at(position);
		liste_string[position]=liste_string.at(position+1);
		liste_string[position+1]=string_temp;

		int int_temp=classement[position];
		classement[position]=classement[position+1];
		classement[position+1]=int_temp;

		modele->setStringList(liste_string);
		vue->setCurrentIndex(modele->index(position+1, 0)); // Sélectionne l'objet qui est descendu
	}
}