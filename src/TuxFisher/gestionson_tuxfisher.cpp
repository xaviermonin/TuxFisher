/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 03/06/2008	
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en sc�ne le personnage Tux
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

#include <iostream>
#include <vector>
#include <QList>
#include <QThread>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include "gestionson_tuxfisher.h"

gestionSon_TuxFisher::gestionSon_TuxFisher()
{
	if (!alutInit(NULL, NULL))
		erreur = tr("Initialisation de la biblioth�que sonore �chou�e: %1").arg("alutInit()");
	musique=false;
	liste_buffer.clear();
	liste_source.clear();
}

gestionSon_TuxFisher::~gestionSon_TuxFisher()
{
	musique=false;
	while(isRunning());
	supprimerSons();
	if (!alutExit ())
		erreur = tr("Fermeture de la biblioth�que sonore �chou�e: %1").arg("alutExit()");
}

void gestionSon_TuxFisher::supprimerSons()
{
	for(int count=0; count<liste_source.count(); count++)
	{
		alSourcei(liste_source[count], AL_BUFFER, 0);
		alDeleteBuffers(1, &liste_buffer[count]);
		alDeleteSources(1, &liste_source[count]);
	}
	liste_buffer.clear();
	liste_source.clear();
}

void gestionSon_TuxFisher::run()
{
	if (!ouvrirFichierOgg("musique/acclimate.ogg"))
    	return;

	const int nb_buffer = 3;
	setPriority(QThread::HighPriority);

    // Cr�ation des deux tampons OpenAL que nous allons utiliser pour la lecture en flux
    
    alGenBuffers(nb_buffer, buffers);

    // On les remplit avec le d�but du son � jouer
    for (int i=0; i<nb_buffer; i++)
		lireOgg(buffers[i], 44100);

    // Cr�ation de la source qui jouera le son
    ALuint Source;
    alGenSources(1, &Source);

    // On ajoute les 2 tampons � la file de la source
    alSourceQueueBuffers(Source, nb_buffer, buffers);

    // On peut commencer la lecture
    alSourcePlay(Source);
	
	ALint Status;
    do
    {
    	// On r�cup�re le nombre de tampons qui ont �t� trait�s
        ALint NbProcessed;
        alGetSourcei(Source, AL_BUFFERS_PROCESSED, &NbProcessed);

        // On les re-remplit et on les r�injecte dans la file
        for (ALint i = 0; i < NbProcessed; i++)
        {
            // On sort le tampon de la file
            ALuint buffer;
            alSourceUnqueueBuffers(Source, 1, &buffer);

            // On le remplit avec les donn�es du fichier
            if (!lireOgg(buffer, 44100))
            {
				std::cerr << "REPETITION" << std::endl;
            	ov_raw_seek(&Stream, 0); // On ne peut plus rien lire, on recommence donc depuis le d�but
            	if(!lireOgg(buffer, 44100))
					std::cerr << "REPETITION ERREUR" << std::endl;
            }
			alSourceQueueBuffers(Source, 1, &buffer);
        }
		alutSleep((ALfloat)0.1);

		alGetSourcei(Source, AL_SOURCE_STATE, &Status);
    }while (musique);
	std::cerr << "SORTIE" << std::endl;
	alSourceStop(Source);
    ALint NbQueued;
	alGetSourcei(Source, AL_BUFFERS_QUEUED, &NbQueued);
	fermerOgg(NbQueued);
}

void gestionSon_TuxFisher::fermerOgg(ALint NbQueued)
{
	// On ferme le fichier ogg-vorbis
    ov_clear(&Stream);

    // Fermeture du fichier
    fclose(fichier);

    // On purge la file de tampons de la source
    ALuint buffer;
    for (ALint i = 0; i < NbQueued; ++i)
        alSourceUnqueueBuffers(Source, 1, &buffer);
    alSourcei(Source, AL_BUFFER, 0);

    // On d�truit les deux tampons
    alDeleteBuffers(2, buffers);

    // On d�truit la source
    alDeleteSources(1, &Source);
}

bool gestionSon_TuxFisher::ouvrirFichierOgg(QString nomfichier)
{
    // Ouverture du fichier
    fichier = fopen(nomfichier.toLatin1().data(), "rb");
    if (!fichier)
    {
		erreur = tr("Impossible d'ouvrir le fichier audio: %1").arg(nomfichier);
        return false;
    }

    // On ouvre un flux ogg-vorbis � partir du fichier que l'on vient d'ouvrir
    int Error = ov_open(fichier, &Stream, NULL, 0);
    if (Error < 0)
    {
		erreur = tr("Impossible de lire les donn�es ogg-vorbis � partir du fichier: %1").arg(nomfichier);
        return false;
    }

    // R�cup�ration des informations du son
    vorbis_info* Infos = ov_info(&Stream, -1);
    format = Infos->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    SampleRate = Infos->rate;

    return true;
}

bool gestionSon_TuxFisher::lireOgg(ALuint buffer, ALsizei nbEchantillons)
{
    // Tableau qui va recevoir les �chantillons lus
    std::vector<ALshort> echantillons(nbEchantillons);

    // D�finition de variables utiles � la lecture
    ALsizei totaleLue = 0;
    ALsizei tailleTotale = nbEchantillons * sizeof(ALshort);
    char *SamplesPtr = reinterpret_cast<char*>(&echantillons[0]);

    // Tant qu'on n'a pas atteint la taille voulue, on lit
    while (totaleLue < tailleTotale)
    {
        // Lecture des �chantillons � partir du flux ogg-vorbis
        ALsizei lue = ov_read(&Stream, SamplesPtr + totaleLue, tailleTotale - totaleLue, 0, 2, 1, NULL);

        if (lue > 0)
            totaleLue += lue; // Pas d'erreur, on comptabilise
        else
            break;
    }
	alBufferData(buffer, format, &echantillons[0], totaleLue, SampleRate);
	//return ( (totaleLue > 0) ? true : false);
	if (totaleLue > 0)
		return true;
	else
		return false;
}

gestionSon_TuxFisher * gestionSon_TuxFisher::getInstance()
{
	if (_singleton == 0)
		_singleton =  new gestionSon_TuxFisher();
	return _singleton;
}

void gestionSon_TuxFisher::kill()
{
	if (_singleton != 0)
	{
		delete _singleton;
		_singleton = 0;
	}
}

son gestionSon_TuxFisher::chargerSon(char *fichier)
{
	ALuint source;
	ALuint buffer;
	buffer = alutCreateBufferFromFile(fichier);
	if (buffer == AL_NONE)
	{
		erreur = tr("Impossible de charger le fichier son: %1").arg(fichier);
		return 0;
	}
	alGenSources (1, &source);
	alSourcei (source, AL_BUFFER, buffer);
	liste_source.append(source);
	liste_buffer.append(buffer);
	return source;
}

void gestionSon_TuxFisher::jouerSon(son source, bool repeter)
{
	if (!source)
		return;
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);
	if (repeter)
		alSourcePlay (source);
	else
	if (status != AL_PLAYING)
	    alSourcePlay (source);
}

void gestionSon_TuxFisher::jouerMusique(bool jouer)
{
	if (jouer)
	{
		musique=true;
		if (!isRunning())
			start();
	}
	else
		musique=false;
}


gestionSon_TuxFisher *gestionSon_TuxFisher::_singleton = 0;
