/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 03/06/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en scène le personnage Tux
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
#include "ChoiceLevelDialog.h"
#include "LevelManager.h"

ChoiceLevelDialog::ChoiceLevelDialog(QWidget *parent)
: QDialog(parent)
{
	// Initialisation graphique
	setMinimumSize(200,100);
	setWindowTitle(tr("Partie à load:"));
	
	combobox_level = new QComboBox(this);
	combobox_difficulty = new QComboBox(this);
	combobox_map = new QComboBox(this);
	button_okay = new QPushButton(tr("OK"), this);
	label_level = new QLabel(tr("Partie: "),this);
	label_difficulty = new QLabel(tr("Difficulté: "),this);
	label_author = new QLabel(tr("Author: "),this);
	line_author = new QLineEdit(this);
	line_author->setEnabled(false);
	label_map = new QLabel(tr("Level: "),this);
	
	groupcombobox = new QGridLayout;
	groupcombobox->addWidget(label_difficulty, 0, 0);
	groupcombobox->addWidget(combobox_difficulty, 0, 1);
	groupcombobox->addWidget(label_level, 1, 0);
	groupcombobox->addWidget(combobox_level, 1, 1);
	groupcombobox->addWidget(label_author, 2, 0);
	groupcombobox->addWidget(line_author, 2, 1);
	groupcombobox->addWidget(label_map, 3, 0);
	groupcombobox->addWidget(combobox_map, 3, 1);
	
	layoutbuttons = new QHBoxLayout;
	layoutbuttons->addStretch();
	layoutbuttons->addWidget(button_okay);
	layoutbuttons->addStretch();
	
	layout = new QVBoxLayout(this);
	layout->addItem(groupcombobox);
	layout->addItem(layoutbuttons);
		
	setLayout(layout);
	
	// Initialisation des texts
	aucunlevel = tr("Aucune partie");
	mapnonunlocked = tr("Level non débloqué");
	difficultes << tr("Tous") << tr("Facile") << tr("Moyen") << tr("Difficile");
	
	LevelManager::getInfosLevelx(list_namefile, list_namelevel,
													list_difficulty, list_nameauthor,
													list_mapsunlocked, list_map);
	
	// Remplacement du nom des maps par "Level non débloqué" si besoin
	for (int i=0; i<list_map.count(); ++i) // List des jeux
		for (int j=0; j<list_map.at(i).count(); j++)
			if (j>=list_mapsunlocked.at(i))
				list_map[i][j]=mapnonunlocked; // On remplace


	// Initialisation du combobox des difficultés
	combobox_difficulty->addItems(difficultes);
	
	// Lancement manuel du chargement et vérification nescessaire la premiere fois
	loadComboBoxLevel();
	loadComboBoxMap();
	checkChoix();
	
	// Connexion des signals et slots
	connect(button_okay, SIGNAL(clicked()), this, SLOT(accept()));
	connect(combobox_level, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(loadComboBoxMap()));
	connect(combobox_difficulty, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(loadComboBoxLevel()));
	connect(combobox_level, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(checkChoix())); // Facultatif ?
	connect(combobox_map, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(checkChoix()));
}

ChoiceLevelDialog::~ChoiceLevelDialog()
{

}

void ChoiceLevelDialog::loadComboBoxLevel()
{
	QString difficulte=combobox_difficulty->currentText();
	combobox_level->clear();
	if (difficulte==difficultes.at(NIV_TOUS)) // TOUS
		combobox_level->addItems(getListNameLevel(NIV_TOUS));
	if (difficulte==difficultes.at(NIV_FACILE)) // Facile
		combobox_level->addItems(getListNameLevel(NIV_FACILE));
	if (difficulte==difficultes.at(NIV_MOYEN)) // Moyen
		combobox_level->addItems(getListNameLevel(NIV_MOYEN));
	if (difficulte==difficultes.at(NIV_DIFFICILE)) // Difficile
		combobox_level->addItems(getListNameLevel(NIV_DIFFICILE));
}

void ChoiceLevelDialog::loadComboBoxMap()
{
	if (combobox_level->currentIndex()!=-1)
	{
		QString difficulte=combobox_difficulty->currentText();
		if (combobox_level->currentText() != aucunlevel)
			line_author->setText(list_nameauthor.at(combobox_level->currentIndex()));
		combobox_map->clear();
		QList<QStringList> maps = getListNameMap((NIVEAU)combobox_difficulty->currentIndex());
		combobox_map->addItems(maps.at(combobox_level->currentIndex()));
	}
}

QStringList ChoiceLevelDialog::getListNameLevel(NIVEAU niv)
{
	QStringList levelx;
	for (int i=0; i < list_difficulty.size(); ++i)
	{
		if (list_difficulty.at(i) == niv || niv == NIV_TOUS)
			levelx << list_namelevel.at(i);
	}
	if (levelx.isEmpty())
	{
		line_author->setText(aucunlevel);
		levelx << aucunlevel;
	}
	return levelx;
}

QList<QStringList> ChoiceLevelDialog::getListNameMap(NIVEAU niv)
{
	QList<QStringList> maps;
	for (int i=0; i < list_difficulty.size(); ++i)
	{
		if (list_difficulty.at(i) == niv || niv == NIV_TOUS) // Ajouts par difficulté
				maps << list_map.at(i);
	}
	if (maps.isEmpty())
		maps << QStringList(aucunlevel);
	return maps;
}

QStringList ChoiceLevelDialog::getListFileName(NIVEAU niv)
{
	QStringList files;
	for (int i=0; i < list_difficulty.size(); ++i)
	{
		if (list_difficulty.at(i) == niv)
			files << list_namefile.at(i);
	}
	return files;
}

QString ChoiceLevelDialog::getFileName()
{
	QString difficulte=combobox_difficulty->currentText();
	if (difficulte==difficultes.at(NIV_TOUS)) // TOUS
		return list_namefile.at(combobox_level->currentIndex());
	if (difficulte==difficultes.at(NIV_FACILE)) // Facile
		return getListFileName(NIV_FACILE).at(combobox_level->currentIndex());
	if (difficulte==difficultes.at(NIV_MOYEN)) // Moyen
		return getListFileName(NIV_MOYEN).at(combobox_level->currentIndex());
	if (difficulte==difficultes.at(NIV_DIFFICILE)) // Difficile
		return getListFileName(NIV_DIFFICILE).at(combobox_level->currentIndex());
	return "";
}

int ChoiceLevelDialog::getNumeroMap()
{
	return combobox_map->currentIndex();
}

QStringList ChoiceLevelDialog::getListNameMap()
{
	QStringList list;
	for (int i=0; i<combobox_map->count(); ++i)
		list << combobox_map->itemText(i);
	return list;
}

void ChoiceLevelDialog::checkChoix()
{
	QString level=combobox_level->currentText();
	QString map=combobox_map->currentText();
	if (level==aucunlevel || map==mapnonunlocked)
		button_okay->setDisabled(true);
	else
		button_okay->setDisabled(false);
}
