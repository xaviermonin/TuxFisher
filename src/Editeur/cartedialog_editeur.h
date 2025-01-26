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

#ifndef _CARTEDIALOG_EDITEUR_H_
#define _CARTEDIALOG_EDITEUR_H_

#include <QDialog>
#include <QLayout>

class QLabel;
class QComboBox;
class QLineEdit;

class carteDialog_Editeur : public QDialog
{
	Q_OBJECT
public:
	carteDialog_Editeur(QWidget *parent=0);
	QString recupererNom();
	QSize recupererTaille();
	void chargerInfo(QString, QSize);
	void chargerListeCarte(QStringList);
private:
	QStringList liste_taille;
	QStringList liste_nomcarte;
	QList<QSize> liste_qsize;
	
	QLabel *label_nom;
	QLabel *label_taille;
	
	QLineEdit *line_nom;
	
	QComboBox *combobox_taille;
	
	QGridLayout *groupe;
	QHBoxLayout *layout_bouton;
	QVBoxLayout *layout;
	
	QPushButton *boutonokay;
private slots:
	void verifierNom();
};

#endif // !_CARTEDIALOG_EDITEUR_H_
