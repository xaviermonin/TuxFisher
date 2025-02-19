/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de niveaux pour TuxFisher
Copyright 2008 - Xavier Monin - Tous droits r�serv�s
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

#ifndef _NIVEAUDIALOG_EDITEUR_H_
#define _NIVEAUDIALOG_EDITEUR_H_

#include <QDialog>
#include <QLayout>

class QLabel;
class QComboBox;
class QLineEdit;

class niveauDialog_Editeur : public QDialog
{
	Q_OBJECT
public:
	niveauDialog_Editeur(QWidget *parent=0);
	void chargerNiveauDialog(QString, QString, int);
	QString recupererNom();
	QString recupererAuteur();
	QString recupererDifficulte();
private:
	QLabel *label_nom;
	QLabel *label_difficulte;
	QLabel *label_auteur;
	
	QLineEdit *line_nom;
	QLineEdit *line_auteur;
	
	QComboBox *combobox_difficulte;
//	QComboBox *combobox_taille;
	
	QGridLayout *groupe;
	QHBoxLayout *layout_bouton;
	QVBoxLayout *layout;
	
	QPushButton *boutonokay;
private slots:
	void verifier();
};

#endif // !_NIVEAUDIALOG_EDITEUR_H_
