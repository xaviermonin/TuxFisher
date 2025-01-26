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
#include <QHttp>
#include <QProgressDialog>
#include <iostream>
#include "../Commun/definitions.h"
#include "telechargerniveaudialog.h"
#include "../Commun/gestionniveau_tuxfisher.h"

telechargerNiveauDialog::telechargerNiveauDialog(QWidget *parent)
: QDialog(parent)
{
	setVisible(false);
	// Initialisation graphique
	setWindowTitle(tr("Télécharger partie"));

	resize(600,300);
	
	http_liste = new QHttp(this);
	http_niveau = new QHttp(this);
	buffer_liste.open(QBuffer::ReadWrite);
	buffer_niveau.open(QBuffer::ReadWrite);

    QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *layout_boutons = new QHBoxLayout;


	// Initialisation des objets
	
	configurerModele();
	configurerVue();

	telechargementencours=-1;

	dialogue_barprogression = new QProgressDialog(this);
	dialogue_barprogression->setModal(true);

	bouton_telecharger = new QPushButton(tr("Télécharger"),this);
	bouton_telecharger->setEnabled(false);
	layout_boutons->addStretch();
	layout_boutons->addWidget(bouton_telecharger);
	layout_boutons->addStretch();

	layout->addWidget(vue);
	layout->addLayout(layout_boutons);

	setLayout(layout);

	connect(http_liste, SIGNAL(done(bool)), this, SLOT(chargerModele(bool)));
	connect(bouton_telecharger, SIGNAL(clicked()), this, SLOT(telechargerNiveau()));
	connect(vue, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(telechargerNiveau()));
	connect(http_niveau, SIGNAL(done(bool)), this, SLOT(telechargementTermine(bool)));
	connect(http_liste, SIGNAL(dataReadProgress(int, int)), this, SLOT(modifierBarProgression(int, int)));
	connect(dialogue_barprogression, SIGNAL(canceled()), http_liste, SLOT(abort()));

	envoyerRequete();
}

telechargerNiveauDialog::~telechargerNiveauDialog()
{
	
}

void telechargerNiveauDialog::configurerModele()
{
	QStringList entete;
	entete << tr("Nom");
	entete << tr("Auteur");
	entete << tr("Difficulté");
	entete << tr("Nb de niveaux");
	entete << tr("Date d'envoi");
	modele = new QStandardItemModel(this);
	modele->setHorizontalHeaderLabels(entete);
}

void telechargerNiveauDialog::configurerVue()
{
	vue = new QTableView;
	vue->setSelectionBehavior(QAbstractItemView::SelectRows);
	vue->setSelectionMode(QAbstractItemView::SingleSelection);
	vue->setSortingEnabled(true);
	vue->setAlternatingRowColors(true);
	vue->setGridStyle(Qt::DashLine);
	vue->setEditTriggers(QAbstractItemView::NoEditTriggers);
	vue->verticalHeader()->hide();
	vue->setModel(modele);
}

void telechargerNiveauDialog::envoyerRequete()
{	
	http_liste->setHost("niveau.tuxfisher.net");
	http_liste->get(QString("/gestion.php?version=")+VERSION_APPLICATION, &buffer_liste);
	dialogue_barprogression->setLabelText(tr("Téléchargement de la liste...."));
}

void telechargerNiveauDialog::modifierBarProgression(int valeur, int maximum)
{
	dialogue_barprogression->setMaximum(maximum);
	dialogue_barprogression->setValue(valeur);
}

void telechargerNiveauDialog::chargerModele(bool erreur)
{
	if (erreur)
	{
		if (!dialogue_barprogression->wasCanceled())
			QMessageBox::information(this,tr("Connexion impossible"),
				tr("Impossible de récupérer la liste des parties sur le serveur.\n"
				   "Vérifiez que vous êtes connecté a Internet et que vous parefeu laisse passer la connexion."),
				   QMessageBox::Ok, QMessageBox::Ok);
		dialogue_barprogression->setVisible(false);
		return;
	}
	QList< QList<QStandardItem *> > liste_standarditems;
	buffer_liste.seek(0);
	QString reponse = buffer_liste.readLine().simplified();
	if (reponse=="MAJ")
	{
		QMessageBox::information(this,tr("Nouvelle version disponible"),
			tr("Vous ne possédez pas la dernière version de %1.\n"
			   "Veuillez mettre à jour votre version pour pouvoir télécharger les parties.").arg(NOM_APPLICATION),
			   QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	else if (reponse=="OK")
	{
		dialogue_barprogression->setLabelText(tr("Création de la liste...."));
		stringlist_id = (QString(buffer_liste.readLine().simplified()).split("@&"));
		stringlist_numeroversion = (QString(buffer_liste.readLine().simplified()).split("@&"));
		if (stringlist_id.count()!=stringlist_numeroversion.count())
		{
			QMessageBox::information(this,tr("Erreur serveur"),
			tr("Le serveur n'a pas répondu correctement."),
			   QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		for (int count_liste=0; count_liste<5; count_liste++)
		{
			QList<QStandardItem *> standarditems;
			QStringList stringlist = (QString(buffer_liste.readLine().simplified()).split("@&"));

			if (count_liste==2) // Réglage difficulté
			{
				for (int count=0; count<stringlist.count(); count++)
					switch(stringlist.at(count).toInt())
					{
						case 1: standarditems << new QStandardItem(tr("%1-Facile").arg(stringlist.at(count))); break;
						case 2: standarditems << new QStandardItem(tr("%1-Moyen").arg(stringlist.at(count))); break;
						case 3: standarditems << new QStandardItem(tr("%1-Difficile").arg(stringlist.at(count))); break;
						default: standarditems << new QStandardItem(tr("%1-Inconnu").arg(stringlist.at(count)));
					}
			}
			else
				for (int count=0; count<stringlist.count(); count++)
					standarditems << new QStandardItem(stringlist.at(count));

			liste_standarditems << standarditems;
		}
	}
	else if (reponse=="VIDE")
		return;
	else
	{
		QMessageBox::information(this,tr("Liste indisponible"),
			tr("Le serveur n\'a pas répondu correctement à la requête."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	
	int nbcolonne=liste_standarditems.count(); // Logiquement 5

	// Filtrage
	dialogue_barprogression->setLabelText(tr("Filtrage de la liste...."));
	for (int count=0; count<liste_standarditems.at(0).count(); count++)
	{
		if (count%30==0)
		{
			if (dialogue_barprogression->wasCanceled())
				return;
			qApp->processEvents();
		}
		gestionNiveau_TuxFisher gestion;
		if ( gestion.chargerFichier(liste_standarditems.at(0).at(count)->text(), // essai de charger le fichier avec le nom du niveau et auteur
			liste_standarditems.at(1).at(count)->text()) ) 
		{ // Le niveau est aussi en local
			qint16 version = stringlist_numeroversion.value(count).toInt(); // numeroversion de la ligne dans le tableau en cours de filtrage
			if (version <= gestion.recupererNumeroVersion())
			{
				stringlist_id.removeAt(count);
				stringlist_numeroversion.removeAt(count);
				for (int i=0; i<nbcolonne; i++)
					liste_standarditems[i].removeAt(count);
				count--;
			}
		}
	}

	// Ajout de la liste
	dialogue_barprogression->setLabelText(tr("Ajout de la liste...."));
	for (int count_colonne=0; count_colonne<nbcolonne; count_colonne++)
	{
		int nbligne=liste_standarditems.at(count_colonne).count();
		for(int count_ligne=0; count_ligne<nbligne; count_ligne++)
		{
			modele->setItem(count_ligne, count_colonne, liste_standarditems.at(count_colonne).at(count_ligne));
			if (count_ligne%30==0)
			{
				if (dialogue_barprogression->wasCanceled())
				{
					modele->clear();
					return;
				}
				qApp->processEvents();
			}
		}
	}
	vue->resizeColumnsToContents();
	std::cerr << "X: " << vue->sizeHint().width() << " Y: " << vue->sizeHint().height();
//	vue->resize(sizeHint());
	//Qt::ScrollBarAlwaysOff
	dialogue_barprogression->setVisible(false);
	bouton_telecharger->setEnabled(true);
}

void telechargerNiveauDialog::telechargerNiveau()
{
	if (telechargementencours!=-1 || !vue->currentIndex().isValid())
		return;
	
	telechargementencours=vue->currentIndex().row();

	std::cerr << "Telechargement en cours...";
	buffer_niveau.reset();
	QString id = stringlist_id.value(vue->currentIndex().row());
	http_niveau->setHost("niveau.tuxfisher.net");
	http_niveau->get("/gestion.php?telecharger="+id, &buffer_niveau);
}

void telechargerNiveauDialog::telechargementTermine(bool erreur)
{
	if (erreur)
	{
		QMessageBox::information(this,tr("Téléchargement échoué"),
			tr("La partie n'a pas pu être téléchargée."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	else
	{
		gestionNiveau_TuxFisher gestion;
		if (!gestion.chargerFichier(buffer_niveau))
			QMessageBox::information(this,tr("Niveau corrompu"),
			tr("%1 n'a pas pu charger la partie téléchargée: %2").arg(NOM_APPLICATION).arg(gestion.recupererErreur()),
			QMessageBox::Ok, QMessageBox::Ok);
		else
		{
			gestion.sauvegarder();
			stringlist_id.removeAt(telechargementencours);
			stringlist_numeroversion.removeAt(telechargementencours);
			modele->removeRow(telechargementencours);
			std::cerr << "Telechargement effectue...";
		}
	}
	telechargementencours=-1;
}
