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

#ifndef _CHOICE_LEVEL_DIALOG_H_
#define _CHOICE_LEVEL_DIALOG_H_

#include <QDialog>
#include <QComboBox>
#include <QLayout>
#include <QLabel>

#include "LevelManager.h"

class ChoiceLevelDialog : public QDialog
{
	Q_OBJECT
public:
	ChoiceLevelDialog(QWidget *parent=0);
	~ChoiceLevelDialog();
	QString getFileName();
	int getNumeroMap();
	QStringList getListNameMap();
private slots:
	void loadComboBoxLevel();
	void loadComboBoxMap();
	void checkChoix();
private:
	QStringList getListNameLevel(NIVEAU);
	QStringList getListFileName(NIVEAU);
	QList<QStringList> getListNameMap(NIVEAU);
	
	QString aucunlevel;
	QString mapnonunlocked;

	QStringList difficultes;

	QStringList list_namefile;
	QStringList list_nameauthor;
	QStringList list_namelevel;
	QList<NIVEAU> list_difficulty;
	QList<int> list_mapsunlocked;
	QList<QStringList> list_map;
	
	QGridLayout *groupcombobox;
	QVBoxLayout *layout;
	QHBoxLayout *layoutbuttons;
	
	QLabel *label_level;
	QLabel *label_author;
	QLabel *label_difficulty;
	QLabel *label_map;
	
	QLineEdit *line_author;
	
	QComboBox *combobox_difficulty;
	QComboBox *combobox_level;
	QComboBox *combobox_map;
	QPushButton *button_okay;
};

#endif // !_CHOIXNIVEAUDIALOG_TUXFISHER_H_
