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
#include "MapDialog.h"

MapDialog::MapDialog(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_name = new QLabel(tr("Name du level: "), this);
	label_size = new QLabel(tr("Size du level: "), this);
	
	QRegExp rx("[0-9a-zA-Z ']{2,30}");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	line_name = new QLineEdit(this);
	line_name->setValidator(validator);
	line_name->setMaxLength(30);
	
	combobox_size = new QComboBox(this);
	
	button_ok = new QPushButton(tr("OK"),this);
	
	// Remplissage des Combobox
	
	list_size << tr("Petite (15x10)") << tr("Moyenne (20x15)") << tr("Grande (30x20)");
	list_qsize << QSize(15,10) << QSize(20,15) << QSize(30,20);
	
	combobox_size->addItems(list_size);
	
	// Positionnement des objets
	group = new QGridLayout;
	group->addWidget(label_name, 0, 0);
	group->addWidget(line_name, 0, 1);
	group->addWidget(label_size, 1, 0);
	group->addWidget(combobox_size, 1, 1);
	
	layout_button = new QHBoxLayout;
	layout_button->addStretch();
	layout_button->addWidget(button_ok);
	
	layout = new QVBoxLayout;
	layout->addItem(group);
	layout->addItem(layout_button);
	
	setLayout(layout);
	
	connect(line_name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
	connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
	
	checkName();
}

void MapDialog::loadInfo(QString nom, QSize size)
{
	line_name->insert(nom);
	for (int count=0; count<list_qsize.size(); count++)
		if (list_qsize.at(count) == size)
			combobox_size->setCurrentIndex(count);
}

void MapDialog::loadMapList(QStringList list)
{
	if (line_name->text()!="")
	{
		for (int c=0; c<list.count(); c++)
		{
			if (line_name->text()==list.at(c))
				list.removeAt(c);
		}
	}
	list_namemap = list;
}

void MapDialog::checkName()
{
	if (line_name->text()=="" || (list_namemap.contains(line_name->text(), Qt::CaseInsensitive)))
		button_ok->setDisabled(true);
	else
		button_ok->setDisabled(false);
}

QString MapDialog::getName()
{
	return line_name->text();
}

QSize MapDialog::getSize()
{
	QSize sizemap;
	return list_qsize.at(combobox_size->currentIndex());
}
