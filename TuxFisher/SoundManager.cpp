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

#include <iostream>
#include <vector>
#include <QList>
#include <QThread>
#include "SoundManager.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
}

void SoundManager::removeSons()
{

}

void SoundManager::run()
{

}


SoundManager * SoundManager::getInstance()
{
	if (_singleton == 0)
		_singleton =  new SoundManager();
	return _singleton;
}

void SoundManager::kill()
{
	if (_singleton != 0)
	{
		delete _singleton;
		_singleton = 0;
	}
}

sound SoundManager::loadSound(char *file)
{
        return 1;
}

void SoundManager::playSound(sound source, bool repeter)
{	
}

void SoundManager::jouerMusique(bool jouer)
{
}


SoundManager *SoundManager::_singleton = 0;
