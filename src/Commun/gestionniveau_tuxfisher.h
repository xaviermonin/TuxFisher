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

#ifndef _GESTIONNIVEAU_TUXFISHER_H_
#define _GESTIONNIVEAU_TUXFISHER_H_

#include <QList>
class QString;
class QSize;
class QFile;
class QByteArray;

// Liste des niveaux de difficulté disponibles
typedef enum { NIV_TOUS=0, NIV_FACILE=1, NIV_MOYEN=2, NIV_DIFFICILE=3 } NIVEAU;

class gestionNiveau_TuxFisher
{
public:
	gestionNiveau_TuxFisher();
	bool nouveauNiveau(QString, QString);
	bool nouvelleCarte(QString, QSize, QByteArray);
	bool chargerFichier(QString);
	bool chargerFichier(QIODevice &);
	bool chargerFichier(QString, QString);
	bool chargerCarte(QByteArray, QSize);
	bool chargerCarte(QByteArray);
	bool supprimerCarte();
	bool selectionnerCarte(int);
	QSize recupererTailleCarte();
	QByteArray recupererDonnesCarte();
	bool echangerCartes(int, int);
	inline int recupererNumeroCarte() { return numerocarte; };
	inline QString recupererNomNiveau() { return nomniveau; }
	inline QString recupererNomAuteur() { return nomauteur; }
	inline QString recupererNomFichier() { return QString(nomrepertoire+nomniveau+"-"+nomauteur+".tf").toLower(); }
	inline QString recupererErreur() { QString erreur_courante=erreur; erreur=""; return erreur_courante; }
	inline NIVEAU recupererDifficulte() { return niveaudifficulte; };
	inline QStringList recupererListeNomCarte() { return nomcarte; };
	inline qint16 recupererNumeroVersion() { return numeroversion; };
	inline void majVersion() { numeroversion+=1; };
	int recupererCartesDebloquees();
	void sauverCartesDebloquees(int);
	QString recupererNomCarte();
	//inline QStringList recupererListeCarte() { return donnes; }
	bool changerNomNiveau(QString);
	bool changerNomAuteur(QString);
	bool changerNomCarte(QString);
	bool changerDifficulte(NIVEAU);
	bool sauvegarder();
	
	static void recupererInfosNiveaux(QStringList &, QStringList &, QList<NIVEAU> &, QStringList &, QList<int> &, QList<QStringList> &);

private:
	int numerocarte; // Index de la carte actuelle
	qint16 numeroversion;
	QString nomniveau;
	QString nomauteur;
	static QString nomrepertoire; // Répertoire des maps
	static QString nomfichierinfoscartes; // Nom du fichier qui contient les infos (si un joueur a terminé tel carte)
	char version;
	QString erreur;
	NIVEAU niveaudifficulte;
	QStringList nomcarte;
	QList<QByteArray> donnes;
	QList<QSize> taillecarte;
};

#endif // !_GESTIONNIVEAU_TUXFISHER_H_
