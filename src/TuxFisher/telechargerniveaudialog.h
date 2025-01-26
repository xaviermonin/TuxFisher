/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
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

#ifndef _TELECHARGERNIVEAUDIALOG_H_
#define _TELECHARGERNIVEAUDIALOG_H_

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QLayout>
#include <QBuffer>
#include <QHttp>
#include <QProgressBar>
#include <QTextEdit>
#include <QProgressDialog>


class telechargerNiveauModel;

class telechargerNiveauDialog : public QDialog
{
	Q_OBJECT
public:
	telechargerNiveauDialog(QWidget *parent=0);
	~telechargerNiveauDialog();
private:
	void configurerModele();
	void configurerVue();
	void envoyerRequete();

	QTableView *vue;
	QStandardItemModel *modele;

	QProgressDialog *dialogue_barprogression;

	QHttp *http_liste;
	QHttp *http_niveau;
	QBuffer buffer_liste;
	QBuffer buffer_niveau;

	QPushButton *bouton_telecharger;

	QStringList stringlist_id;
	QStringList stringlist_numeroversion;

	int telechargementencours;

private slots:
	void chargerModele(bool);
	void telechargerNiveau();
	void telechargementTermine(bool erreur);
	void modifierBarProgression(int, int);
};

#endif // !_TELECHARGERNIVEAUDIALOG_H_
