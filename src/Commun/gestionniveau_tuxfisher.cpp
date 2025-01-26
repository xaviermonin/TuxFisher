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
#include <iostream>
#include "gestionniveau_tuxfisher.h"
#include "definitions.h"

gestionNiveau_TuxFisher::gestionNiveau_TuxFisher()
{
	numerocarte = -1; // Niveau actuel (commence à partir de 0)
	version = 0;
}

bool gestionNiveau_TuxFisher::sauvegarder()
{
	QFile fichier(recupererNomFichier());
	QDateTime date = QDateTime::currentDateTime();
	QByteArray bytearray_numerodeversion;
	if (fichier.open(QFile::WriteOnly))
	{
		char caract_difficulte;
		char caract_x;
		char caract_y;
		char caract_nb;
		fichier.write("[TuxFisher]");
		fichier.write(VERSION_CARTE.toLatin1().data());
		std::cerr << "Numero version: " << numeroversion;
		fichier.write((char*)&numeroversion, 2);
		fichier.write(nomniveau.toLatin1()+"\n");
		fichier.write(nomauteur.toLatin1()+"\n");
		caract_difficulte=niveaudifficulte;
		caract_nb=nomcarte.size();
		fichier.write(&caract_difficulte,1);
		fichier.write(&caract_nb,1);
		
		for (int count=0; count<caract_nb; count++)
		{
			caract_x=taillecarte.at(count).width();
			caract_y=taillecarte.at(count).height();
			fichier.write(&caract_x,1);
			fichier.write(&caract_y,1);
			fichier.write(nomcarte.at(count).toLatin1()+"\n");
			fichier.write(donnes.at(count));
		}
		fichier.close();
	}
	else
		return false;
	return true;
}

int gestionNiveau_TuxFisher::recupererCartesDebloquees()
{
	QFile fichierinfos(nomrepertoire+nomfichierinfoscartes);
	if (fichierinfos.open(QIODevice::ReadWrite))
	{
		while(!fichierinfos.atEnd())
		{
			QString string=fichierinfos.readLine().simplified();
			QStringList stringliste=string.split(",");
			if (stringliste.count()==2)
				if (stringliste[0]==recupererNomFichier())
					return stringliste[1].toInt();
		}
	}
	return 1;
}

void gestionNiveau_TuxFisher::sauverCartesDebloquees(int cartesdebloquees)
{
	QFile fichier_lecture(nomrepertoire+nomfichierinfoscartes);
	QStringList infosnomsfichiers;
	QList<int> infoscartesdebloquees;

	// On stock toutes les infos des cartes
	if (fichier_lecture.open(QIODevice::ReadWrite))
	{
		while(!fichier_lecture.atEnd())
		{
			QString string=fichier_lecture.readLine().simplified();
			QStringList stringliste=string.split(",");
			if (stringliste.count()==2)
				if (stringliste[0]!=recupererNomFichier())
				{
					infosnomsfichiers << stringliste[0];
					infoscartesdebloquees << stringliste[1].toInt();
				}
		}
		fichier_lecture.close();
	}

	// On enregistre tout
	QFile fichier_ecriture(nomrepertoire+nomfichierinfoscartes);
	if (fichier_ecriture.open(QIODevice::WriteOnly))
	{
		QString string_cartesdebloquees;
		string_cartesdebloquees.setNum(cartesdebloquees);
		QString string(recupererNomFichier()+","+string_cartesdebloquees+"\r\n");
		fichier_ecriture.write(string.toLatin1().data());
		for (int count=0; count<infosnomsfichiers.count(); count++)
		{
			string_cartesdebloquees.setNum(infoscartesdebloquees[count]);
			QString string(infosnomsfichiers[count]+","+string_cartesdebloquees+"\r\n");
			fichier_ecriture.write(string.toLatin1().data());
		}
		fichier_ecriture.close();
	}
}

bool gestionNiveau_TuxFisher::nouveauNiveau(QString niveau, QString auteur)
{
	numerocarte= -1;
	nomniveau = niveau;
	nomauteur = auteur;
	nomcarte.clear();
	donnes.clear();
	taillecarte.clear();
	numeroversion = 0;
	return true;
}

bool gestionNiveau_TuxFisher::nouvelleCarte(QString carte, QSize taille, QByteArray donnescarte)
{
	if (carte.isEmpty())
		return false;
	if (taille.width()*taille.height() != donnescarte.size())
		return false;
	nomcarte << carte;
	taillecarte << taille;
	donnes << donnescarte;
	numerocarte=nomcarte.size()-1;
	return true;
}

bool gestionNiveau_TuxFisher::chargerFichier(QString fichier)
{
	QFile fichierniveau(fichier);
	if (!fichierniveau.open(QIODevice::ReadOnly))
	{
		erreur = QObject::tr("Le fichier %1 n'a pas été ouvert.").arg(fichier);
		return false;
	}
	int retour = chargerFichier(fichierniveau);
	
	fichierniveau.close();
	return retour;
}

bool gestionNiveau_TuxFisher::chargerFichier(QString niveau, QString auteur)
{
	QString nomfichier=QString((nomrepertoire+niveau+"-"+auteur+".tf").toLower());
	int retour = chargerFichier(nomfichier);
	
	return retour;
}

bool gestionNiveau_TuxFisher::chargerFichier(QIODevice &fichierniveau)
{
	if (!fichierniveau.isOpen())
	{
		erreur = QObject::tr("Le fichier du jeu n'a pas été ouvert.");
		return false;
	}
	int nbcarte;
	char caract;
	QByteArray controle;
	
	numerocarte=-1;
	nomniveau.clear();
	nomcarte.clear();
	donnes.clear();
	taillecarte.clear();
	
	fichierniveau.seek(0);
	
	QString chaineclef = fichierniveau.read(11);
	if (chaineclef=="[TuxFisher]")
	{
		// Lit la version du niveau
		QString version = fichierniveau.read(8);
		if (version!=VERSION_CARTE)
		{
			erreur = QObject::tr("La version de la partie n'est pas supportée.");
			return false;
		}
		// Lit le numero de version
		fichierniveau.read((char*) &numeroversion, 2);
		// Lit le nom du niveau
		nomniveau = fichierniveau.readLine().simplified();
		if (nomniveau.isEmpty())
		{
			erreur = QObject::tr("La partie n'a pas de nom.");
			return false;
		}
		// Lit le nom de l'auteur
		nomauteur = fichierniveau.readLine().simplified();
		if (nomauteur.isEmpty())
		{
			erreur = QObject::tr("La partie n'a pas d'auteur.");
			return false;
		}
		// Lit le niveau de difficulté du niveau
		fichierniveau.read(&caract, 1);
		niveaudifficulte = (NIVEAU)caract;
		// Lit le nombre de cartes du niveau
		fichierniveau.read(&caract, 1);
		nbcarte = caract;
		// - Partie qui lit les noms de carte du niveau -
		for (int c=0; c<nbcarte; c++)
		{
			QSize taille;
			// Lit la taille d'une carte
			fichierniveau.read(&caract, 1);
			taille.setWidth(caract);
			fichierniveau.read(&caract, 1);
			taille.setHeight(caract);
			taillecarte << taille;
			int nboctets = taille.width()*taille.height();
			// Lit le nom d'une carte
			nomcarte << fichierniveau.readLine().simplified();
			// Lit les données de la carte
			QByteArray octets(fichierniveau.read(nboctets));
			if (octets.size()!=nboctets)
			{
				erreur = QObject::tr("Les niveaux du jeu n'ont pas pu être lu correctement.");
				return false;
			}
			donnes << octets;
		}
		if (nbcarte != nomcarte.size())
		{
			erreur = QObject::tr("Les niveau lus ne correspondent pas au nombre de niveau présents");
			return false;
		}
	}
	else
	{
		erreur = QObject::tr("Le fichier n'est pas un partie %1.").arg(NOM_APPLICATION);
		return false;
	}
	numerocarte=0; // = 1 carte
	return true;
}

bool gestionNiveau_TuxFisher::supprimerCarte()
{
	if (nomcarte.size()>1) // Si plus d'une carte existe
	{
		nomcarte.removeAt(numerocarte);
		donnes.removeAt(numerocarte);
		taillecarte.removeAt(numerocarte);
		if (numerocarte>0)
		{
			numerocarte--;
			selectionnerCarte(numerocarte);
		}
		else
		{
			numerocarte = 0;
			selectionnerCarte(numerocarte);
		}
		return true;
	}
	else
		return false; // Pas assez de carte, donc rien à supprimer
}

bool gestionNiveau_TuxFisher::selectionnerCarte(int num)
{
	if (nomcarte.size()>num && num>=0)
	{
		numerocarte = num;
		return true;
	}
	else
		return false;
}

bool gestionNiveau_TuxFisher::chargerCarte(QByteArray carte)
{
	if (donnes.size()>=numerocarte)
	{
		donnes.replace(numerocarte, carte);
		return true;
	}
	return false;
}

bool gestionNiveau_TuxFisher::chargerCarte(QByteArray carte, QSize taille)
{
	if (donnes.size()>=numerocarte)
	{
		if (taille.width()*taille.height() != carte.size())
			return false;
		donnes.replace(numerocarte, carte);
		taillecarte.replace(numerocarte, taille);
		return true;
	}
	return false;
}

bool gestionNiveau_TuxFisher::changerNomCarte(QString nom)
{
	if (nomcarte.size()>=numerocarte)
		if (!nom.isEmpty())
		{
			nomcarte.replace(numerocarte, nom);
			return true;
		}
	return false;
}

bool gestionNiveau_TuxFisher::changerNomNiveau(QString nom)
{
	if (!nom.isEmpty())
	{
		nomniveau = nom;
		return true;
	}
	return false;
}

bool gestionNiveau_TuxFisher::changerNomAuteur(QString nom)
{
	if (!nom.isEmpty())
	{
		nomauteur = nom;
		return true;
	}
	return false;
}

bool gestionNiveau_TuxFisher::changerDifficulte(NIVEAU niv)
{
	niveaudifficulte=niv;
	return true;
}

bool gestionNiveau_TuxFisher::echangerCartes(int premier, int second)
{
	int indexselection=numerocarte; // Carte sélectionnée

	if (premier>=nomcarte.size() && premier<0)
		return false;
	if (second>=nomcarte.size() && second<0)
		return false;

	QString string_temp=nomcarte.at(premier);
	QSize size_temp=taillecarte.at(premier);
	QByteArray bytearray_temp=donnes.at(premier);

	nomcarte[premier]=nomcarte[second];
	taillecarte[premier]=taillecarte[second];
	donnes[premier]=donnes[second];

	nomcarte[second]=string_temp;
	taillecarte[second]=size_temp;
	donnes[second]=bytearray_temp;

	selectionnerCarte(indexselection); // On revient à la carte selectionnée avant

	return true;
}

QByteArray gestionNiveau_TuxFisher::recupererDonnesCarte()
{
	if (donnes.size()<numerocarte)
		return donnes.at(0);
	else
		return donnes.at(numerocarte);
}

QSize gestionNiveau_TuxFisher::recupererTailleCarte()
{
	if (taillecarte.size()<numerocarte)
		return taillecarte.at(0);
	else
		return taillecarte.at(numerocarte);		
}

QString gestionNiveau_TuxFisher::recupererNomCarte()
{
	if (nomcarte.size()<numerocarte)
		return "";
	else
		return nomcarte.at(numerocarte);
}

// Fonction statique renvoyant les informations de tous les niveaux du répertoire maps
void gestionNiveau_TuxFisher::recupererInfosNiveaux(QStringList &liste_nomfichier,
	QStringList &liste_nomniveau, QList<NIVEAU> &liste_difficulte, QStringList &liste_nomauteur,
	QList<int> &liste_cartesdebloquees, QList<QStringList> &liste_nomcarte)
{
	QDir repertoire(nomrepertoire);
	QStringList liste_repertoire = repertoire.entryList((QStringList)"*.tf", QDir::Files);
	gestionNiveau_TuxFisher gestion;
	for (int i = 0; i < liste_repertoire.size(); ++i)
	{
		QString fichier(nomrepertoire+(QString)QDir::separator()+liste_repertoire.at(i));
		QStringList nomcartes;
		if(gestion.chargerFichier(fichier))
		{
			liste_nomfichier << fichier;
			liste_nomcarte << gestion.recupererListeNomCarte();
			liste_nomniveau << gestion.recupererNomNiveau();
			liste_difficulte << gestion.recupererDifficulte();
			liste_cartesdebloquees << gestion.recupererCartesDebloquees();
			liste_nomauteur << gestion.recupererNomAuteur();
		}
	}
}

QString gestionNiveau_TuxFisher::nomrepertoire="maps"+(QString)QDir::separator();
QString gestionNiveau_TuxFisher::nomfichierinfoscartes="cartes.cfg";
