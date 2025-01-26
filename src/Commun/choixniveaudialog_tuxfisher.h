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

#ifndef _CHOIXNIVEAUDIALOG_TUXFISHER_H_
#define _CHOIXNIVEAUDIALOG_TUXFISHER_H_

#include <QDialog>
#include <QComboBox>
#include <QLayout>
#include <QLabel>

#include "gestionniveau_tuxfisher.h"

class choixNiveauDialog_TuxFisher : public QDialog
{
	Q_OBJECT
public:
	choixNiveauDialog_TuxFisher(QWidget *parent=0);
	~choixNiveauDialog_TuxFisher();
	QString recupererNomFichier();
	int recupererNumeroCarte();
	QStringList recupererListeNomCarte();
private slots:
	void chargerComboBoxNiveau();
	void chargerComboBoxCarte();
	void verifierChoix();
private:
	QStringList recupererListeNomNiveau(NIVEAU);
	QStringList recupererListeNomFichier(NIVEAU);
	QList<QStringList> recupererListeNomCarte(NIVEAU);
	
	QString aucunniveau;
	QString cartenondebloquees;

	QStringList difficultes;

	QStringList liste_nomfichier;
	QStringList liste_nomauteur;
	QStringList liste_nomniveau;
	QList<NIVEAU> liste_difficulte;
	QList<int> liste_cartesdebloquees;
	QList<QStringList> liste_carte;
	
	QGridLayout *groupecombobox;
	QVBoxLayout *layout;
	QHBoxLayout *layoutboutons;
	
	QLabel *label_niveau;
	QLabel *label_auteur;
	QLabel *label_difficulte;
	QLabel *label_carte;
	
	QLineEdit *line_auteur;
	
	QComboBox *combobox_difficulte;
	QComboBox *combobox_niveau;
	QComboBox *combobox_carte;
	QPushButton *bouton_okay;
};

#endif // !_CHOIXNIVEAUDIALOG_TUXFISHER_H_
