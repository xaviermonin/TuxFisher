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

#include <QtGui>
#include "choixniveaudialog_tuxfisher.h"
#include "gestionniveau_tuxfisher.h"

choixNiveauDialog_TuxFisher::choixNiveauDialog_TuxFisher(QWidget *parent)
: QDialog(parent)
{
	// Initialisation graphique
	setMinimumSize(200,100);
	setWindowTitle(tr("Partie à charger:"));
	
	combobox_niveau = new QComboBox(this);
	combobox_difficulte = new QComboBox(this);
	combobox_carte = new QComboBox(this);
	bouton_okay = new QPushButton(tr("OK"), this);
	label_niveau = new QLabel(tr("Partie: "),this);
	label_difficulte = new QLabel(tr("Difficulté: "),this);
	label_auteur = new QLabel(tr("Auteur: "),this);
	line_auteur = new QLineEdit(this);
	line_auteur->setEnabled(false);
	label_carte = new QLabel(tr("Niveau: "),this);
	
	groupecombobox = new QGridLayout;
	groupecombobox->addWidget(label_difficulte, 0, 0);
	groupecombobox->addWidget(combobox_difficulte, 0, 1);
	groupecombobox->addWidget(label_niveau, 1, 0);
	groupecombobox->addWidget(combobox_niveau, 1, 1);
	groupecombobox->addWidget(label_auteur, 2, 0);
	groupecombobox->addWidget(line_auteur, 2, 1);
	groupecombobox->addWidget(label_carte, 3, 0);
	groupecombobox->addWidget(combobox_carte, 3, 1);
	
	layoutboutons = new QHBoxLayout;
	layoutboutons->addStretch();
	layoutboutons->addWidget(bouton_okay);
	layoutboutons->addStretch();
	
	layout = new QVBoxLayout(this);
	layout->addItem(groupecombobox);
	layout->addItem(layoutboutons);
		
	setLayout(layout);
	
	// Initialisation des textes
	aucunniveau = tr("Aucune partie");
	cartenondebloquees = tr("Niveau non débloqué");
	difficultes << tr("Tous") << tr("Facile") << tr("Moyen") << tr("Difficile");
	
	gestionNiveau_TuxFisher::recupererInfosNiveaux(liste_nomfichier, liste_nomniveau,
													liste_difficulte, liste_nomauteur,
													liste_cartesdebloquees, liste_carte);
	
	// Remplacement du nom des cartes par "Niveau non débloqué" si besoin
	for (int i=0; i<liste_carte.count(); i++) // Liste des jeux
		for (int j=0; j<liste_carte.at(i).count(); j++)
			if (j>=liste_cartesdebloquees.at(i))
				liste_carte[i][j]=cartenondebloquees; // On remplace


	// Initialisation du combobox des difficultés
	combobox_difficulte->addItems(difficultes);
	
	// Lancement manuel du chargement et vérification nescessaire la premiere fois
	chargerComboBoxNiveau();
	chargerComboBoxCarte();
	verifierChoix();
	
	// Connexion des signals et slots
	connect(bouton_okay, SIGNAL(clicked()), this, SLOT(accept()));
	connect(combobox_niveau, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(chargerComboBoxCarte()));
	connect(combobox_difficulte, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(chargerComboBoxNiveau()));
	connect(combobox_niveau, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(verifierChoix())); // Facultatif ?
	connect(combobox_carte, SIGNAL(currentIndexChanged(QString)),
			this, SLOT(verifierChoix()));
}

choixNiveauDialog_TuxFisher::~choixNiveauDialog_TuxFisher()
{

}

void choixNiveauDialog_TuxFisher::chargerComboBoxNiveau()
{
	QString difficulte=combobox_difficulte->currentText();
	combobox_niveau->clear();
	if (difficulte==difficultes.at(NIV_TOUS)) // TOUS
		combobox_niveau->addItems(recupererListeNomNiveau(NIV_TOUS));
	if (difficulte==difficultes.at(NIV_FACILE)) // Facile
		combobox_niveau->addItems(recupererListeNomNiveau(NIV_FACILE));
	if (difficulte==difficultes.at(NIV_MOYEN)) // Moyen
		combobox_niveau->addItems(recupererListeNomNiveau(NIV_MOYEN));
	if (difficulte==difficultes.at(NIV_DIFFICILE)) // Difficile
		combobox_niveau->addItems(recupererListeNomNiveau(NIV_DIFFICILE));
}

void choixNiveauDialog_TuxFisher::chargerComboBoxCarte()
{
	if (combobox_niveau->currentIndex()!=-1)
	{
		QString difficulte=combobox_difficulte->currentText();
		if (combobox_niveau->currentText() != aucunniveau)
			line_auteur->setText(liste_nomauteur.at(combobox_niveau->currentIndex()));
		combobox_carte->clear();
		QList<QStringList> cartes = recupererListeNomCarte((NIVEAU)combobox_difficulte->currentIndex());
		combobox_carte->addItems(cartes.at(combobox_niveau->currentIndex()));
	}
}

QStringList choixNiveauDialog_TuxFisher::recupererListeNomNiveau(NIVEAU niv)
{
	QStringList niveaux;
	for (int i=0; i < liste_difficulte.size(); i++)
	{
		if (liste_difficulte.at(i) == niv || niv == NIV_TOUS)
			niveaux << liste_nomniveau.at(i);
	}
	if (niveaux.isEmpty())
	{
		line_auteur->setText(aucunniveau);
		niveaux << aucunniveau;
	}
	return niveaux;
}

QList<QStringList> choixNiveauDialog_TuxFisher::recupererListeNomCarte(NIVEAU niv)
{
	QList<QStringList> cartes;
	for (int i=0; i < liste_difficulte.size(); i++)
	{
		if (liste_difficulte.at(i) == niv || niv == NIV_TOUS) // Ajouts par difficulté
				cartes << liste_carte.at(i);
	}
	if (cartes.isEmpty())
		cartes << QStringList(aucunniveau);
	return cartes;
}

QStringList choixNiveauDialog_TuxFisher::recupererListeNomFichier(NIVEAU niv)
{
	QStringList fichiers;
	for (int i=0; i < liste_difficulte.size(); i++)
	{
		if (liste_difficulte.at(i) == niv)
			fichiers << liste_nomfichier.at(i);
	}
	return fichiers;
}

QString choixNiveauDialog_TuxFisher::recupererNomFichier()
{
	QString difficulte=combobox_difficulte->currentText();
	if (difficulte==difficultes.at(NIV_TOUS)) // TOUS
		return liste_nomfichier.at(combobox_niveau->currentIndex());
	if (difficulte==difficultes.at(NIV_FACILE)) // Facile
		return recupererListeNomFichier(NIV_FACILE).at(combobox_niveau->currentIndex());
	if (difficulte==difficultes.at(NIV_MOYEN)) // Moyen
		return recupererListeNomFichier(NIV_MOYEN).at(combobox_niveau->currentIndex());
	if (difficulte==difficultes.at(NIV_DIFFICILE)) // Difficile
		return recupererListeNomFichier(NIV_DIFFICILE).at(combobox_niveau->currentIndex());
	return "";
}

int choixNiveauDialog_TuxFisher::recupererNumeroCarte()
{
	return combobox_carte->currentIndex();
}

QStringList choixNiveauDialog_TuxFisher::recupererListeNomCarte()
{
	QStringList liste;
	for (int i=0; i<combobox_carte->count(); i++)
		liste << combobox_carte->itemText(i);
	return liste;
}

void choixNiveauDialog_TuxFisher::verifierChoix()
{
	QString niveau=combobox_niveau->currentText();
	QString carte=combobox_carte->currentText();
	if (niveau==aucunniveau || carte==cartenondebloquees)
		bouton_okay->setDisabled(true);
	else
		bouton_okay->setDisabled(false);
}
