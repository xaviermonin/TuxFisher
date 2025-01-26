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

#include <QtGui>
#include <QHttp>
#include <iostream>
#include "mettreenlignedialog_editeur.h"
#include "../Commun/definitions.h"

mettreEnLigneDialog_Editeur::mettreEnLigneDialog_Editeur(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_nomfichier = new QLabel(this);
	barprogression = new QProgressBar(this);
	boutonenvoyer = new QPushButton(tr("Envoyer"),this);
	
	
	hote = "niveau.tuxfisher.net";
	destinationupload = "/upload.php";
	http = new QHttp(this);
	
	// Positionnement des objets
	groupe = new QGridLayout;
	groupe->addWidget(label_nomfichier, 0, 0);
		
	layout_bouton = new QHBoxLayout;
	layout_bouton->addStretch();
	layout_bouton->addWidget(boutonenvoyer);
	layout_bouton->addStretch();
	
	layout = new QVBoxLayout(this);
	layout->addItem(groupe);
	layout->addWidget(barprogression);
	layout->addItem(layout_bouton);
	
	setLayout(layout);
	
	ecraser=false;
	
	connect(http, SIGNAL(dataSendProgress(int,int)), this, SLOT(modifierBarProgression(int,int)));
	connect(http, SIGNAL(done(bool)), this, SLOT(controleRequete(bool)));
	connect(boutonenvoyer, SIGNAL(clicked()), this, SLOT(envoyerfichier()));
}

mettreEnLigneDialog_Editeur::~mettreEnLigneDialog_Editeur()
{
	delete layout_bouton;
	delete groupe;
}

void mettreEnLigneDialog_Editeur::modifierBarProgression(int courant, int maximum)
{
	barprogression->setMaximum(maximum);
	barprogression->setValue(courant);
}

void mettreEnLigneDialog_Editeur::controleRequete(bool erreur)
{
	QByteArray data(http->readAll());
	QString retour(data);
	boutonenvoyer->disconnect();
	boutonenvoyer->setText(tr("Fermer"));
	connect(boutonenvoyer, SIGNAL(clicked()), this, SLOT(accept()));
	if (erreur)
	{
		QMessageBox::information(this, tr("Probl�me r�seau"),
		tr("Le serveur %1 n'a pas pu �tre contact�.\r\nAssurez vous d'�tre correctement connect� � Internet.")
		.arg(hote));
		return;
	}
	else if (retour!="OK")
	{
		if (retour=="ERREUR NIVEAU EXISTANT")
		{
			int retour = QMessageBox::question(this, tr("Jeu existant"), tr("Ce jeu existe d�ja sur le serveur, voulez vous l'�craser ?"),
			QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape);
			if (retour == QMessageBox::No)
				return;
			ecraser=true;
			envoyerfichier();
			return;
		}
		if (retour=="ERREUR CLIENT")
		{
			QMessageBox::information(this, tr("Client erron�"),
			tr("Le client (%1) n'as pas �t� reconnu par le serveur."
			" Assurez vous d'avoir la derni�re version de %1.").arg(NOM_APPLICATION).arg(NOM_APPLICATION));
			return;
		}
		if (retour=="ERREUR CLIENT VERSION")
		{
			QMessageBox::information(this, tr("Version du client incompatible"),
			tr("La version du client (%1) n'est plus support�e par le serveur. T�l�chargez la derni�re version de %2.").arg(NOM_APPLICATION).arg(NOM_APPLICATION));
			return;
		}
		if (retour=="ERREUR VERSION")
		{
			QMessageBox::information(this, tr("Version non prise en charge"),
			tr("La version actuelle de %1 n'est pas prise en charge par le serveur.\r\n"
			"Veuillez mettre � jour %2.").arg(NOM_APPLICATION).arg(NOM_APPLICATION));
			return;
		}
		if (retour=="ERREUR UPLOAD")
		{
			QMessageBox::information(this, tr("Mise en ligne impossible"),
			tr("La mise en ligne du fichier a �chou�."));
			return;
		}
		if (retour=="ERREUR COPIE")
		{
			QMessageBox::information(this, tr("Probl�me de serveur"),
			tr("Le serveur a re�u le fichier mais n'as pas pu le stocker."));
			return;
		}
		if (retour=="ERREUR NIVEAU")
		{
			QMessageBox::information(this, tr("Jeu corrompu"),
			tr("Le jeu n'a pas �t� v�rifi� avec succ�s par le serveur. Verifiez votre version de %1 ou r�essayez.").arg(NOM_APPLICATION));
			return;
		}
		if (retour=="ERREUR NIVEAU VERSION")
		{
			QMessageBox::information(this, tr("Jeu non g�r�"),
			tr("Ce jeu n'est plus g�r� par le serveur. Veuillez mettre � jour %1.").arg(NOM_APPLICATION));
			return;
		}
		std::cerr << retour.toLatin1().data();
		QMessageBox::information(this, tr("Erreur du serveur"),
		tr("Le serveur n'a pas r�pondu correctement � la requ�te.\r\n"
		"Assurez vous d'avoir la derni�re version du programme et que le serveur soit op�rationnel."));
		return;
	}
}

void mettreEnLigneDialog_Editeur::chargerNomFichier(QString nom)
{
	nomfichiercomplet=nom;
	QFileInfo infofichier(nomfichiercomplet);
	nomfichier = infofichier.fileName();
	label_nomfichier->setText(tr("Nom fichier: %1").arg(nomfichier));
}

void mettreEnLigneDialog_Editeur::envoyerfichier()
{
	if (nomfichiercomplet.isEmpty())
		return;
	QFile fichier(nomfichiercomplet);
	if (!fichier.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, tr("Erreur - Ouverture du fichier"),
		tr("Impossible d'ouvrir le fichier %1: %2.")
		.arg(nomfichiercomplet).arg(fichier.errorString()));
		return;
	}

	// Cr�ation de l'ent�te HTTP
	QHttpRequestHeader header("POST", "/"+destinationupload, 1, 1);
	header.setValue("Host", hote);
	header.setValue("Content-type", "multipart/form-data, boundary=AaB03x");
	header.setValue("User-Agent", "TuxFisher "+VERSION_APPLICATION);
	header.setValue("Cache-Control", "no-cache");
	header.setValue("Accept","*/*");
	
	// Cr�ation des donn�es � envoyer
	QByteArray donnes;
	donnes.append("--AaB03x\r\n");
	donnes.append("content-disposition: ");
	donnes.append("form-data; name=\"upload_maptf\"\r\n");
	donnes.append("\r\n");
	donnes.append("0\r\n");
	// Ajouter ici
	if (ecraser==true)
	{
		donnes.append("--AaB03x\r\n");
		donnes.append("content-disposition: ");
		donnes.append("form-data; name=\"ecraser\"\r\n");
		donnes.append("\r\n");
		donnes.append("0\r\n");
		ecraser=false;
	}
	donnes.append("--AaB03x\r\n");
	donnes.append("content-disposition: ");
	donnes.append("form-data; name=\"fichier\"; filename=\""+ nomfichier +"\"\r\n");
	donnes.append("Content-Transfer-Encoding: binary\r\n");
	donnes.append("\r\n");
	donnes.append(fichier.readAll());
	fichier.close();
	donnes.append("\r\n");
	donnes.append("--AaB03x--");
	
	int longueurdonnes = donnes.length();
	header.setContentLength(longueurdonnes);

	http->setHost(hote);
	http->request(header, donnes);
}
