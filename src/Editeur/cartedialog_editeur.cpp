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

#include <QtGui>
#include "cartedialog_editeur.h"

carteDialog_Editeur::carteDialog_Editeur(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_nom = new QLabel(tr("Nom du niveau: "), this);
	label_taille = new QLabel(tr("Taille du niveau: "), this);
	
	QRegExp rx("[0-9a-zA-Z ']{2,30}");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	line_nom = new QLineEdit(this);
	line_nom->setValidator(validator);
	line_nom->setMaxLength(30);
	
	combobox_taille = new QComboBox(this);
	
	boutonokay = new QPushButton(tr("OK"),this);
	
	// Remplissage des Combobox
	
	liste_taille << tr("Petite (15x10)") << tr("Moyenne (20x15)") << tr("Grande (30x20)");
	liste_qsize << QSize(15,10) << QSize(20,15) << QSize(30,20);
	
	combobox_taille->addItems(liste_taille);
	
	// Positionnement des objets
	groupe = new QGridLayout;
	groupe->addWidget(label_nom, 0, 0);
	groupe->addWidget(line_nom, 0, 1);
	groupe->addWidget(label_taille, 1, 0);
	groupe->addWidget(combobox_taille, 1, 1);
	
	layout_bouton = new QHBoxLayout;
	layout_bouton->addStretch();
	layout_bouton->addWidget(boutonokay);
	
	layout = new QVBoxLayout;
	layout->addItem(groupe);
	layout->addItem(layout_bouton);
	
	setLayout(layout);
	
	connect(line_nom, SIGNAL(textChanged(QString)), this, SLOT(verifierNom()));
	connect(boutonokay, SIGNAL(clicked()), this, SLOT(accept()));
	
	verifierNom();
}

void carteDialog_Editeur::chargerInfo(QString nom, QSize taille)
{
	line_nom->insert(nom);
	for (int count=0; count<liste_qsize.size(); count++)
		if (liste_qsize.at(count) == taille)
			combobox_taille->setCurrentIndex(count);
}

void carteDialog_Editeur::chargerListeCarte(QStringList liste)
{
	if (line_nom->text()!="")
	{
		for (int c=0; c<liste.count(); c++)
		{
			if (line_nom->text()==liste.at(c))
				liste.removeAt(c);
		}
	}
	liste_nomcarte = liste;
}

void carteDialog_Editeur::verifierNom()
{
	if (line_nom->text()=="" || (liste_nomcarte.contains(line_nom->text(), Qt::CaseInsensitive)))
		boutonokay->setDisabled(true);
	else
		boutonokay->setDisabled(false);
}

QString carteDialog_Editeur::recupererNom()
{
	return line_nom->text();
}

QSize carteDialog_Editeur::recupererTaille()
{
	QSize taillecarte;
	return liste_qsize.at(combobox_taille->currentIndex());
}
