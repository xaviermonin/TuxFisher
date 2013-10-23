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

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

typedef enum { CASE_AVALANCHE=33, CASE_VIDE, CASE_TUXFISHER, CASE_NEIGE,
			   CASE_POISSON, CASE_MONTAGNE, ERREUR=-1} CASE;
typedef enum { MVT_HAUT, MVT_BAS, MVT_GAUCHE, MVT_DROITE} MVT;
typedef enum { RET_MONTAGNE, RET_AVALANCHE, RET_POISSON, RET_NEIGE, RET_VIDE, RET_RETOUR, RET_RIEN } RETOURMVT;

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    CASE getCase(int, int);
    CASE getCaseMapOptimisation(int, int);
    bool fallingAvalanche();
    RETOURMVT moveTuxFisher(MVT);
    RETOURMVT getBackToPrevMap();
    bool dropAvalanche();
    inline bool gameOver() { return game_over; };
    bool levelCompleted();
    bool loadMap(int, int, char*);
    void addPreviousMap();
    void loadMapOptimisation();

private:
    int COORD_X;
    int COORD_Y;
    int MAX_RETOUR;
    MVT dernier_mouvement;
    int avalanche_x;
    int avalanche_y;
	int tuxfisher_x;
	int tuxfisher_y;
    bool game_over;
    CASE **map;
    CASE ***mappreviouse;
    CASE **mapoptimisation;
    
    void reset();
    void initPreviousMaps();
};

#endif
