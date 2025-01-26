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

#ifndef _GESTIONSON_TUXFISHER_H_
#define _GESTIONSON_TUXFISHER_H_

#include <QThread>
#include <QList>
#include <vorbis/vorbisfile.h>
#include <AL/alut.h>

typedef ALuint son;
typedef ALuint musique;

class gestionSon_TuxFisher : public QThread
{
	Q_OBJECT

private:
	gestionSon_TuxFisher();
	~gestionSon_TuxFisher();
	
	bool ouvrirFichierOgg(QString);
	bool lireOgg(ALuint, ALsizei);
	void fermerOgg(ALint);
public:
	static gestionSon_TuxFisher *getInstance();
	static void kill();
	son chargerSon(char *);
	void jouerSon(son, bool repeter=true);
	void supprimerSons();
	inline QString retourErreur() { QString tmp=erreur; erreur=""; return tmp; };
protected:
	void run();
private:
	static gestionSon_TuxFisher *_singleton;
	QList<ALuint> liste_buffer;
	QList<ALuint> liste_source;
	bool musique;

	OggVorbis_File Stream;
	FILE *fichier;
	ALenum format;
	ALsizei SampleRate;
	ALuint buffers[2];
	ALuint Source;
	QString erreur;
private slots:
	void jouerMusique(bool);
};

#endif // !_GESTIONSON_TUXFISHER_H_
