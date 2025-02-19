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

#ifndef _METTREENLIGNEDIALOG_EDITEUR_H_
#define _METTREENLIGNEDIALOG_EDITEUR_H_

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QHttp>
#include <QProgressBar>


class mettreEnLigneDialog_Editeur : public QDialog
{
	Q_OBJECT
public:
	mettreEnLigneDialog_Editeur(QWidget *parent=0);
	~mettreEnLigneDialog_Editeur();
	void chargerNomFichier(QString);
private:
	bool ecraser;
	QString nomfichiercomplet;
	QString nomfichier;
	QString hote;
	QString destinationupload;
	
	QLabel *label_nomfichier;
	QProgressBar *barprogression;
	
	QGridLayout *groupe;
	QHBoxLayout *layout_bouton;
	QVBoxLayout *layout;
	
	QPushButton *boutonenvoyer;
//	QPushButton *boutonfermer;

	QHttp *http;
private slots:
	void envoyerfichier();
	void modifierBarProgression(int, int);
	void controleRequete(bool);
};

#endif // !_METTREENLIGNEDIALOG_EDITEUR_H_
