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

#include <QtGui>
#include <QHttp>
#include <QProgressDialog>
#include <iostream>
#include "../Commun/definitions.h"
#include "LevelDownloaderDialog.h"
#include "../Commun/LevelManager.h"

LevelDownloaderDialog::LevelDownloaderDialog(QWidget *parent)
: QDialog(parent)
{
	setVisible(false);
	// Initialisation graphique
	setWindowTitle(tr("Téléload partie"));

	resize(600,300);
	
	http_list = new QHttp(this);
	http_level = new QHttp(this);
	buffer_list.open(QBuffer::ReadWrite);
	buffer_level.open(QBuffer::ReadWrite);

    QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *layout_buttons = new QHBoxLayout;


	// Initialisation des objets
	
	configurerModele();
	configurerVue();

	telechargementencours=-1;

	dialogue_progress_bar = new QProgressDialog(this);
	dialogue_progress_bar->setModal(true);

	button_teleload = new QPushButton(tr("Téléload"),this);
	button_teleload->setEnabled(false);
	layout_buttons->addStretch();
	layout_buttons->addWidget(button_teleload);
	layout_buttons->addStretch();

	layout->addWidget(vue);
	layout->addLayout(layout_buttons);

	setLayout(layout);

	connect(http_list, SIGNAL(done(bool)), this, SLOT(loadModele(bool)));
	connect(button_teleload, SIGNAL(clicked()), this, SLOT(downloadLevel()));
	connect(vue, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(downloadLevel()));
	connect(http_level, SIGNAL(done(bool)), this, SLOT(telechargementTermine(bool)));
	connect(http_list, SIGNAL(dataReadProgress(int, int)), this, SLOT(setProgressBar(int, int)));
	connect(dialogue_progress_bar, SIGNAL(canceled()), http_list, SLOT(abort()));

	sendRequete();
}

LevelDownloaderDialog::~LevelDownloaderDialog()
{
	
}

void LevelDownloaderDialog::configurerModele()
{
	QStringList entete;
	entete << tr("Name");
	entete << tr("Author");
	entete << tr("Difficulté");
	entete << tr("Nb de levelx");
	entete << tr("Date d'envoi");
	modele = new QStandardItemModel(this);
	modele->setHorizontalHeaderLabels(entete);
}

void LevelDownloaderDialog::configurerVue()
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

void LevelDownloaderDialog::sendRequete()
{	
	http_list->setHost("level.tuxfisher.net");
	http_list->get(QString("/gestion.php?version=")+VERSION_APPLICATION, &buffer_list);
	dialogue_progress_bar->setLabelText(tr("Téléchargement de la list...."));
}

void LevelDownloaderDialog::setProgressBar(int valeur, int maximum)
{
	dialogue_progress_bar->setMaximum(maximum);
	dialogue_progress_bar->setValue(valeur);
}

void LevelDownloaderDialog::loadModele(bool error)
{
	if (error)
	{
		if (!dialogue_progress_bar->wasCanceled())
			QMessageBox::information(this,tr("Connexion impossible"),
				tr("Impossible de récupérer la list des parties sur le serveur.\n"
				   "Vérifiez que vous êtes connecté a Internet et que vous parefeu laisse passer la connexion."),
				   QMessageBox::Ok, QMessageBox::Ok);
		dialogue_progress_bar->setVisible(false);
		return;
	}
	QList< QList<QStandardItem *> > list_standarditems;
	buffer_list.seek(0);
	QString reponse = buffer_list.readLine().simplified();
	if (reponse=="MAJ")
	{
		QMessageBox::information(this,tr("Nouvelle version disponible"),
			tr("Vous ne possédez pas la dernière version de %1.\n"
			   "Veuillez mettre à jour votre version pour pouvoir téléload les parties.").arg(APPLICATION_NAME),
			   QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	else if (reponse=="OK")
	{
		dialogue_progress_bar->setLabelText(tr("Création de la list...."));
		stringlist_id = (QString(buffer_list.readLine().simplified()).split("@&"));
		stringlist_numeroversion = (QString(buffer_list.readLine().simplified()).split("@&"));
		if (stringlist_id.count()!=stringlist_numeroversion.count())
		{
			QMessageBox::information(this,tr("Error serveur"),
			tr("Le serveur n'a pas répondu correctement."),
			   QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		for (int count_list=0; count_list<5; count_list++)
		{
			QList<QStandardItem *> standarditems;
			QStringList stringlist = (QString(buffer_list.readLine().simplified()).split("@&"));

			if (count_list==2) // Réglage difficulté
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

			list_standarditems << standarditems;
		}
	}
	else if (reponse=="VIDE")
		return;
	else
	{
		QMessageBox::information(this,tr("List indisponible"),
			tr("Le serveur n\'a pas répondu correctement à la requête."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	
	int nbcolonne=list_standarditems.count(); // Logiquement 5

	// Filtrage
	dialogue_progress_bar->setLabelText(tr("Filtrage de la list...."));
	for (int count=0; count<list_standarditems.at(0).count(); count++)
	{
		if (count%30==0)
		{
			if (dialogue_progress_bar->wasCanceled())
				return;
			qApp->processEvents();
		}
		LevelManager gestion;
		if ( gestion.loadFile(list_standarditems.at(0).at(count)->text(), // essai de load le file avec le nom du level et author
			list_standarditems.at(1).at(count)->text()) ) 
		{ // Le level est aussi en local
			qint16 version = stringlist_numeroversion.value(count).toInt(); // numeroversion de la ligne dans le tableau en cours de filtrage
			if (version <= gestion.getNumeroVersion())
			{
				stringlist_id.removeAt(count);
				stringlist_numeroversion.removeAt(count);
				for (int i=0; i<nbcolonne; ++i)
					list_standarditems[i].removeAt(count);
				count--;
			}
		}
	}

	// Ajout de la list
	dialogue_progress_bar->setLabelText(tr("Ajout de la list...."));
	for (int count_colonne=0; count_colonne<nbcolonne; count_colonne++)
	{
		int nbligne=list_standarditems.at(count_colonne).count();
		for(int count_ligne=0; count_ligne<nbligne; count_ligne++)
		{
			modele->setItem(count_ligne, count_colonne, list_standarditems.at(count_colonne).at(count_ligne));
			if (count_ligne%30==0)
			{
				if (dialogue_progress_bar->wasCanceled())
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
	dialogue_progress_bar->setVisible(false);
	button_teleload->setEnabled(true);
}

void LevelDownloaderDialog::downloadLevel()
{
	if (telechargementencours!=-1 || !vue->currentIndex().isValid())
		return;
	
	telechargementencours=vue->currentIndex().row();

	std::cerr << "Telechargement en cours...";
	buffer_level.reset();
	QString id = stringlist_id.value(vue->currentIndex().row());
	http_level->setHost("level.tuxfisher.net");
	http_level->get("/gestion.php?teleload="+id, &buffer_level);
}

void LevelDownloaderDialog::telechargementTermine(bool error)
{
	if (error)
	{
		QMessageBox::information(this,tr("Téléchargement échoué"),
			tr("La partie n'a pas pu être téléchargée."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	else
	{
		LevelManager gestion;
		if (!gestion.loadFile(buffer_level))
			QMessageBox::information(this,tr("Level corrompu"),
			tr("%1 n'a pas pu load la partie téléchargée: %2").arg(APPLICATION_NAME).arg(gestion.getError()),
			QMessageBox::Ok, QMessageBox::Ok);
		else
		{
			gestion.save();
			stringlist_id.removeAt(telechargementencours);
			stringlist_numeroversion.removeAt(telechargementencours);
			modele->removeRow(telechargementencours);
			std::cerr << "Telechargement effectue...";
		}
	}
	telechargementencours=-1;
}
