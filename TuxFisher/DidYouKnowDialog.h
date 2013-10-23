/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 03/06/2010
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en scène le personnage Tux
Copyright 2010 - Xavier Monin - Tous droits réservés
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

#ifndef _DID_YOU_DIALOG_H_
#define _DID_YOU_DIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
class QPushButton;

class DidYouKnowDialog : public QDialog
{
	Q_OBJECT
public:
	DidYouKnowDialog(QWidget *parent=0);
	~DidYouKnowDialog();
	bool displayed() { return !checkbox_afficher->isChecked(); };
	void setCheckBox(bool etat) { checkbox_afficher->setChecked(etat); };
	void displayWelcome();

private:
	void loadLabel();

	int index;
	QLabel *label_text;
	QLabel *label_image;
	QCheckBox *checkbox_afficher;
	QPushButton *button_close;
	QPushButton *button_previous;
	QPushButton *button_next;
	
	QStringList list;
private slots:
	void previous();
	void next();
};

#endif // !_SAVIEZVOUSDIALOG_H_
