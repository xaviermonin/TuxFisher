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

#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <QThread>
#include <QList>

typedef int sound;
typedef int music;

class SoundManager : public QThread
{
	Q_OBJECT

private:
	SoundManager();
	~SoundManager();

public:
	static SoundManager* getInstance();
	static void kill();
	sound loadSound(char *);
	void playSound(sound, bool repeter=true);
	void removeSons();
    inline QString retourError() { return ""; };
protected:
	void run();
private:
	static SoundManager* _singleton;
	bool music;

private slots:
	void jouerMusique(bool);
};

#endif // !_GESTIONNAIRE_SON_H_
