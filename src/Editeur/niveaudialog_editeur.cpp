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
#include "niveaudialog_editeur.h"

niveauDialog_Editeur::niveauDialog_Editeur(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_nom = new QLabel(tr("Nom du jeu: "), this);
	label_auteur = new QLabel(tr("Nom de l'auteur: "), this);
	label_difficulte = new QLabel(tr("Niveau de difficulté: "), this);

	QRegExp rx("[0-9a-zA-Z ']{2,25}");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	line_nom = new QLineEdit(this);
	line_nom->setValidator(validator);
	line_nom->setMaxLength(30);

	line_auteur = new QLineEdit(this);
	line_auteur->setValidator(validator);
	line_auteur->setMaxLength(30);
	
	combobox_difficulte = new QComboBox(this);
	
	boutonokay = new QPushButton(tr("OK"),this);
	
	// Remplissage des Combobox
	
	QStringList liste_difficulte;
	liste_difficulte << tr("Facile") << tr("Moyen") << tr("Difficile");
	
	combobox_difficulte->addItems(liste_difficulte);
	
	// Positionnement des objets
	groupe = new QGridLayout;
	groupe->addWidget(label_nom, 0, 0);
	groupe->addWidget(line_nom, 0, 1);
	groupe->addWidget(label_auteur, 1, 0);
	groupe->addWidget(line_auteur, 1, 1);
	groupe->addWidget(label_difficulte, 2, 0);
	groupe->addWidget(combobox_difficulte, 2, 1);
		
	layout_bouton = new QHBoxLayout;
	layout_bouton->addStretch();
	layout_bouton->addWidget(boutonokay);
	layout_bouton->addStretch();
	
	layout = new QVBoxLayout;
	layout->addItem(groupe);
	layout->addItem(layout_bouton);
	
	setLayout(layout);
	
	connect(line_nom, SIGNAL(textChanged(QString)), this, SLOT(verifier()));
	connect(line_auteur, SIGNAL(textChanged(QString)), this, SLOT(verifier()));
	connect(boutonokay, SIGNAL(clicked()), this, SLOT(accept()));
	
	verifier();
}

void niveauDialog_Editeur::chargerNiveauDialog(QString nom, QString auteur, int difficulte)
{
	line_nom->setText(nom);
	line_auteur->setText(auteur);
	if (difficulte>=0 && difficulte<4)
		combobox_difficulte->setCurrentIndex(difficulte-1);
}

void niveauDialog_Editeur::verifier()
{
	if (line_nom->text()=="" || line_auteur->text()=="")
		boutonokay->setDisabled(true);
	else
		boutonokay->setDisabled(false);
}

QString niveauDialog_Editeur::recupererNom()
{
	return line_nom->text();
}

QString niveauDialog_Editeur::recupererAuteur()
{
	return line_auteur->text();
}

QString niveauDialog_Editeur::recupererDifficulte()
{
	return combobox_difficulte->currentText();
}
