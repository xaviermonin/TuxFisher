/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 03/06/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en sc�ne le personnage Tux
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

#ifndef _SAVIEZVOUSDIALOG_H_
#define _SAVIEZVOUSDIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
class QPushButton;

class saviezVousDialog : public QDialog
{
	Q_OBJECT
public:
	saviezVousDialog(QWidget *parent=0);
	~saviezVousDialog();
	bool afficher() { return !checkbox_afficher->isChecked(); };
	void ChargerEtatCheckBox(bool etat) { checkbox_afficher->setChecked(etat); };
	void afficherPresentation();

private:
	void chargerLabel();

	int index;
	QLabel *label_texte;
	QLabel *label_image;
	QCheckBox *checkbox_afficher;
	QPushButton *bouton_fermer;
	QPushButton *bouton_precedent;
	QPushButton *bouton_suivant;
	
	QStringList liste;
private slots:
	void precedent();
	void suivant();
};

#endif // !_SAVIEZVOUSDIALOG_H_
