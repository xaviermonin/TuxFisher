/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de levelx pour TuxFisher
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
#include "LevelUploader.h"
#include "../Commun/definitions.h"

LevelUploader::LevelUploader(QWidget *parent)
: QDialog(parent)
{
	// Initialisation des objets
	label_namefile = new QLabel(this);
	progress_bar = new QProgressBar(this);
	buttonsend = new QPushButton(tr("Envoyer"),this);
	
	
	host = "level.tuxfisher.net";
	destinationupload = "/upload.php";
	http = new QHttp(this);
	
	// Positionnement des objets
	group = new QGridLayout;
	group->addWidget(label_namefile, 0, 0);
		
	layout_button = new QHBoxLayout;
	layout_button->addStretch();
	layout_button->addWidget(buttonsend);
	layout_button->addStretch();
	
	layout = new QVBoxLayout(this);
	layout->addItem(group);
	layout->addWidget(progress_bar);
	layout->addItem(layout_button);
	
	setLayout(layout);
	
	overwrite=false;
	
	connect(http, SIGNAL(dataSendProgress(int,int)), this, SLOT(setProgressBar(int,int)));
	connect(http, SIGNAL(done(bool)), this, SLOT(checkRequest(bool)));
	connect(buttonsend, SIGNAL(clicked()), this, SLOT(sendfile()));
}

LevelUploader::~LevelUploader()
{
	delete layout_button;
	delete group;
}

void LevelUploader::setProgressBar(int courant, int maximum)
{
	progress_bar->setMaximum(maximum);
	progress_bar->setValue(courant);
}

void LevelUploader::checkRequest(bool error)
{
	QByteArray data(http->readAll());
	QString retour(data);
	buttonsend->disconnect();
	buttonsend->setText(tr("Fermer"));
	connect(buttonsend, SIGNAL(clicked()), this, SLOT(accept()));
	if (error)
	{
		QMessageBox::information(this, tr("Probl�me r�seau"),
		tr("Le serveur %1 n'a pas pu �tre contact�.\r\nAssurez vous d'�tre correctement connect� � Internet.")
		.arg(host));
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
			overwrite=true;
			sendfile();
			return;
		}
		if (retour=="ERREUR CLIENT")
		{
			QMessageBox::information(this, tr("Client erron�"),
			tr("Le client (%1) n'as pas �t� reconnu par le serveur."
			" Assurez vous d'avoir la derni�re version de %1.").arg(NAME_APPLICATION).arg(NAME_APPLICATION));
			return;
		}
		if (retour=="ERREUR CLIENT VERSION")
		{
			QMessageBox::information(this, tr("Version du client incompatible"),
			tr("La version du client (%1) n'est plus support�e par le serveur. T�l�chargez la derni�re version de %2.").arg(NAME_APPLICATION).arg(NAME_APPLICATION));
			return;
		}
		if (retour=="ERREUR VERSION")
		{
			QMessageBox::information(this, tr("Version non prise en charge"),
			tr("La version actuelle de %1 n'est pas prise en charge par le serveur.\r\n"
			"Veuillez mettre � jour %2.").arg(NAME_APPLICATION).arg(NAME_APPLICATION));
			return;
		}
		if (retour=="ERREUR UPLOAD")
		{
			QMessageBox::information(this, tr("Mise en ligne impossible"),
			tr("La mise en ligne du file a �chou�."));
			return;
		}
		if (retour=="ERREUR COPIE")
		{
			QMessageBox::information(this, tr("Probl�me de serveur"),
			tr("Le serveur a re�u le file mais n'as pas pu le stocker."));
			return;
		}
		if (retour=="ERREUR NIVEAU")
		{
			QMessageBox::information(this, tr("Jeu corrompu"),
			tr("Le jeu n'a pas �t� v�rifi� avec succ�s par le serveur. Verifiez votre version de %1 ou r�essayez.").arg(NAME_APPLICATION));
			return;
		}
		if (retour=="ERREUR NIVEAU VERSION")
		{
			QMessageBox::information(this, tr("Jeu non g�r�"),
			tr("Ce jeu n'est plus g�r� par le serveur. Veuillez mettre � jour %1.").arg(NAME_APPLICATION));
			return;
		}
		std::cerr << retour.toLatin1().data();
		QMessageBox::information(this, tr("Error du serveur"),
		tr("Le serveur n'a pas r�pondu correctement � la requ�te.\r\n"
		"Assurez vous d'avoir la derni�re version du programme et que le serveur soit op�rationnel."));
		return;
	}
}

void LevelUploader::loadFileName(QString nom)
{
	fullfilename=nom;
	QFileInfo infofile(fullfilename);
	filename = infofile.fileName();
	label_namefile->setText(tr("Name file: %1").arg(filename));
}

void LevelUploader::sendfile()
{
	if (fullfilename.isEmpty())
		return;
	QFile file(fullfilename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, tr("Error - Ouverture du file"),
		tr("Impossible d'ouvrir le file %1: %2.")
		.arg(fullfilename).arg(file.errorString()));
		return;
	}

	// Cr�ation de l'ent�te HTTP
	QHttpRequestHeader header("POST", "/"+destinationupload, 1, 1);
	header.setValue("Host",host);
	header.setValue("Content-type", "multipart/form-data, boundary=AaB03x");
	header.setValue("User-Agent", "TuxFisher "+VERSION_APPLICATION);
	header.setValue("Cache-Control", "no-cache");
	header.setValue("Accept","*/*");
	
	// Cr�ation des donn�es � send
	QByteArray datas;
	datas.append("--AaB03x\r\n");
	datas.append("content-disposition: ");
	datas.append("form-data; name=\"upload_maptf\"\r\n");
	datas.append("\r\n");
	datas.append("0\r\n");
	// Ajouter ici
	if (overwrite==true)
	{
		datas.append("--AaB03x\r\n");
		datas.append("content-disposition: ");
		datas.append("form-data; name=\"overwrite\"\r\n"); // overwrite ??
		datas.append("\r\n");
		datas.append("0\r\n");
		overwrite=false;
	}
	datas.append("--AaB03x\r\n");
	datas.append("content-disposition: ");
	datas.append("form-data; name=\"file\"; filename=\""+ filename +"\"\r\n");
	datas.append("Content-Transfer-Encoding: binary\r\n");
	datas.append("\r\n");
	datas.append(file.readAll());
	file.close();
	datas.append("\r\n");
	datas.append("--AaB03x--");
	
	int widthdatas = datas.length();
	header.setContentLength(widthdatas);

	http->setHost(host);
	http->request(header, datas);
}
