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

#include <QtGui>
#include "LevelDialog.h"

LevelDialog::LevelDialog(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_name = new QLabel(tr("Name du jeu: "), this);
	label_author = new QLabel(tr("Name de l'author: "), this);
	label_difficulty = new QLabel(tr("Level de difficulté: "), this);

	QRegExp rx("[0-9a-zA-Z ']{2,25}");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	line_name = new QLineEdit(this);
	line_name->setValidator(validator);
	line_name->setMaxLength(30);

	line_author = new QLineEdit(this);
	line_author->setValidator(validator);
	line_author->setMaxLength(30);
	
	combobox_difficulty = new QComboBox(this);
	
	button_ok = new QPushButton(tr("OK"),this);
	
	// Remplissage des Combobox
	
	QStringList list_difficulty;
	list_difficulty << tr("Facile") << tr("Moyen") << tr("Difficile");
	
	combobox_difficulty->addItems(list_difficulty);
	
	// Positionnement des objets
	group = new QGridLayout;
	group->addWidget(label_name, 0, 0);
	group->addWidget(line_name, 0, 1);
	group->addWidget(label_author, 1, 0);
	group->addWidget(line_author, 1, 1);
	group->addWidget(label_difficulty, 2, 0);
	group->addWidget(combobox_difficulty, 2, 1);
		
	layout_button = new QHBoxLayout;
	layout_button->addStretch();
	layout_button->addWidget(button_ok);
	layout_button->addStretch();
	
	layout = new QVBoxLayout;
	layout->addItem(group);
	layout->addItem(layout_button);
	
	setLayout(layout);
	
	connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(check()));
	connect(line_author, SIGNAL(textChanged(QString)), this, SLOT(check()));
	connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
	
	check();
}

void LevelDialog::loadLevelDialog(QString nom, QString author, int difficulte)
{
	line_name->setText(nom);
	line_author->setText(author);
	if (difficulte>=0 && difficulte<4)
		combobox_difficulty->setCurrentIndex(difficulte-1);
}

void LevelDialog::check()
{
	if (line_name->text()=="" || line_author->text()=="")
		button_ok->setDisabled(true);
	else
		button_ok->setDisabled(false);
}

QString LevelDialog::getName()
{
	return line_name->text();
}

QString LevelDialog::getAuthor()
{
	return line_author->text();
}

QString LevelDialog::getDifficulty()
{
	return combobox_difficulty->currentText();
}
