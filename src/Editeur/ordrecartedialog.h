/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade 2D mettant en scène le personnage Tux
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

#ifndef _ORDRECARTEDIALOG_H_
#define _ORDRECARTEDIALOG_H_

#include <QDialog>
#include <QLayout>
#include <QMap>
class QListView;
class QStringListModel;

class ordreCarteDialog : public QDialog
{
	Q_OBJECT
public:
	ordreCarteDialog(QWidget *parent=0);
	~ordreCarteDialog();
	void chargerStringList(QStringList);
	inline QMap <int, int> recupererClassement() { return classement; };
private:
	QListView *vue;
	QStringListModel *modele;
	QMap <int, int> classement;
	QStringList liste_string;

	QPushButton *bouton_ok;
	QPushButton *bouton_haut;
	QPushButton *bouton_bas;
private slots:
	void monterOrdre();
	void descendreOrdre();
};

#endif // !_ORDRECARTEDIALOG_H_
