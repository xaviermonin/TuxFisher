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

#ifndef _LEVEL_DIALOG_H_
#define _LEVEL_DIALOG_H_

#include <QDialog>
#include <QLayout>

class QLabel;
class QComboBox;
class QLineEdit;

class LevelDialog : public QDialog
{
	Q_OBJECT
public:
	LevelDialog(QWidget *parent=0);
	void loadLevelDialog(QString, QString, int);
	QString getName();
	QString getAuthor();
	QString getDifficulty();
private:
	QLabel *label_name;
	QLabel *label_difficulty;
	QLabel *label_author;
	
	QLineEdit *line_name;
	QLineEdit *line_author;
	
	QComboBox *combobox_difficulty;
//	QComboBox *combobox_size;
	
	QGridLayout *group;
	QHBoxLayout *layout_button;
	QVBoxLayout *layout;
	
	QPushButton *button_ok;
private slots:
	void check();
};

#endif // !_NIVEAUDIALOG_EDITEUR_H_
