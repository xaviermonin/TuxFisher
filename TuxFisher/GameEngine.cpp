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

#include "GameEngine.h"

#ifndef NULL
#define NULL 0
#endif

GameEngine::GameEngine()
{
	map = NULL;
	mappreviouse = NULL;
	mapoptimisation = NULL;
	MAX_RETOUR = 20;
}

GameEngine::~GameEngine()
{
	reset();
}

void GameEngine::reset()
{
	game_over = false;

	// Destruction de la map si besoin
	if (map != NULL)
	{
	    for(int i = 0; i < COORD_X; ++i)
			delete map[i];
	    delete [] map;
	}

	if (mappreviouse != NULL)
	{
		for(int i = 0; i < MAX_RETOUR; ++i)
		{
        	for (int c = 0; c < COORD_X; ++c)
				delete mappreviouse[i][c];
				
			delete mappreviouse[i];
        }
		
	    delete [] mappreviouse;
	}

	if (mapoptimisation != NULL)
	{
	    for(int i = 0; i<COORD_X ; ++i)
			delete mapoptimisation[i];

	    delete [] mapoptimisation;
	}
	
	mappreviouse = NULL;
	map = NULL;
	mapoptimisation = NULL;
}

void GameEngine::initPreviousMaps()
{
	mappreviouse = new CASE**[MAX_RETOUR];
    for(int i = 0; i < MAX_RETOUR; ++i)
	{
		mappreviouse[i] = new CASE*[COORD_X];
		for (int c = 0; c < COORD_X; ++c)
			mappreviouse[i][c] = new CASE[COORD_Y];
	}
	for (int i = 0; i < MAX_RETOUR; ++i)
		mappreviouse[i][0][0]=ERREUR;
}

RETOURMVT GameEngine::getBackToPrevMap()
{
	if (mappreviouse == NULL)
		return RET_RIEN;
		
	if (mappreviouse[0][0][0] == ERREUR)
		return RET_RIEN;

	for (int a = 0; a < COORD_X; ++a)
		for(int b = 0; b < COORD_Y; ++b)
			map[a][b] = mappreviouse[0][a][b];

	for (int a = 1; a < MAX_RETOUR; ++a)
		for (int b = 0; b < COORD_X; ++b)
			for (int c = 0; c < COORD_Y; ++c)
				mappreviouse[a-1][b][c]=mappreviouse[a][b][c];
	
	for (int a = 0; a < COORD_X; ++a)
		for(int b = 0; b < COORD_Y; ++b)
			mappreviouse[MAX_RETOUR-1][a][b] = ERREUR;

	for (int countx = 0; countx < COORD_X; ++countx) // Calcul position TuxFisher
	{
        for (int county = 0; county < COORD_Y; ++county)
		{
            if (map[countx][county] == CASE_TUXFISHER)
			{
				tuxfisher_x = countx;
				tuxfisher_y = county;
			}
		}
	}

	return RET_RETOUR;
}

void GameEngine::addPreviousMap()
{
	if (MAX_RETOUR > 1)
	{
		for (int a = (MAX_RETOUR-2); a >= 0; --a)
			for (int b = 0; b < COORD_X; ++b)
				for (int c = 0; c < COORD_Y; ++c)
					mappreviouse[a+1][b][c] = mappreviouse[a][b][c];
	}
	
	for (int a = 0; a < COORD_X; ++a)
			for(int b = 0; b < COORD_Y; ++b)
				mappreviouse[0][a][b] = map[a][b];
}

void GameEngine::loadMapOptimisation()
{
	if (mapoptimisation == NULL)
		return;
	
	for (int a = 0; a < COORD_X; ++a)
	{
		for (int b = 0; b < COORD_Y; ++b)
		{
			if (mapoptimisation[a][b] != map[a][b])			
				mapoptimisation[a][b] = map[a][b];
		}
	}
}

bool GameEngine::loadMap(int x, int y, char *datasmap)
{
	// Réinitialisation
	reset();
	
	int sizemap = NULL;
	if (x<2 || y<2) // Ne peut pas être créé car map en 1D
		return false;

	for (int c = 0; datasmap[c] != NULL; ++c) // Calcul size réelle de la map
		sizemap++;
	if (sizemap!=x*y)
		return false;

    COORD_X = x;
    COORD_Y = y;

    initPreviousMaps();
    int count = 0;
    map = new CASE*[COORD_X]; // Création map
    for(int i = 0 ; i<COORD_X ; ++i)
        map[i] = new CASE[COORD_Y];

	mapoptimisation = new CASE*[COORD_X];
    for(int i = 0 ; i<COORD_X ; ++i)
        mapoptimisation[i] = new CASE[COORD_Y];
	mapoptimisation[0][0]=ERREUR;

	for (int countx = 0; countx < COORD_X ; countx++) // Chargement de la map
        for (int county = 0; county < COORD_Y ; county++)
        {
           	map[countx][county] = (CASE)(datasmap[count]);
           	count++;
    	}

	for (int countx = 0; countx < COORD_X ; countx++) // Calcul position TuxFisher
        for (int county = 0; county < COORD_Y ; county++)
            if (map[countx][county]==CASE_TUXFISHER)
			{
				tuxfisher_x=countx;
				tuxfisher_y=county;
			}

    return true;
}

CASE GameEngine::getCase(int x, int y)
{
	if (x>=COORD_X || y>=COORD_Y)
		return ERREUR;
	if (x<0 || y<0)
		return ERREUR;
	if (map == NULL)
		return ERREUR;
	return map[x][y];
}

CASE GameEngine::getCaseMapOptimisation(int x, int y)
{
	if (x>=COORD_X || y>=COORD_Y)
		return ERREUR;
	if (mapoptimisation == NULL)
		return ERREUR;
	return mapoptimisation[x][y];
}

bool GameEngine::levelCompleted()
{
	if (!game_over)
	{
		for (int x = NULL; x<COORD_X ; x++)
			for (int y = NULL ; y<COORD_Y ; y++)
				if (map[x][y]==CASE_POISSON)
					return false;
		return true;
	}
	else
		return false;
}

RETOURMVT GameEngine::moveTuxFisher(MVT deplacement)
{
	int x = 0;
	int y = 0;
	RETOURMVT retour = RET_RIEN;
	if (game_over)
		return retour;

	switch(deplacement)
	{
		case MVT_HAUT: y=-1; break;
        case MVT_BAS: y=1; break;
        case MVT_GAUCHE: x=-1; break;
        case MVT_DROITE: x=1; break;
        default: return retour;
	}
	if (tuxfisher_x+x < COORD_X && tuxfisher_y+y < COORD_Y &&
		tuxfisher_x+x >= 0 && tuxfisher_y+y >= 0)
	{
		// Configuration par défaut des retours
		switch (map[tuxfisher_x+x][tuxfisher_y+y])
		{
			case CASE_NEIGE: retour = RET_NEIGE; break;
			case CASE_POISSON: retour = RET_POISSON; break;
			case CASE_VIDE: retour = RET_VIDE; break;
			case CASE_MONTAGNE: retour = RET_MONTAGNE; break;
			case CASE_AVALANCHE: retour = RET_AVALANCHE; break;
			default: ;
		}
        	
		// On effectue un mouvement simple ?
		if (map[tuxfisher_x+x][tuxfisher_y+y] == CASE_NEIGE ||
			map[tuxfisher_x+x][tuxfisher_y+y] == CASE_POISSON ||
			map[tuxfisher_x+x][tuxfisher_y+y] == CASE_VIDE)
		{
			if (tuxfisher_y-1>= NULL && deplacement==MVT_BAS) // Empeche de se suicider (appuyer sur down avec une boule en up)
				if (map[tuxfisher_x][tuxfisher_y-1] == CASE_AVALANCHE)
					return RET_RIEN;

			addPreviousMap();
			map[tuxfisher_x][tuxfisher_y]=CASE_VIDE;
			map[tuxfisher_x+x][tuxfisher_y+y]=CASE_TUXFISHER;
			dernier_mouvement=deplacement;
			tuxfisher_x+=x;
			tuxfisher_y+=y;
			return retour;
		}

		// On pousse une boule de neige ?
		if (map[tuxfisher_x+x][tuxfisher_y]==CASE_AVALANCHE &&
			(deplacement==MVT_GAUCHE || deplacement==MVT_DROITE))
			if (tuxfisher_x+(2*x) < COORD_X && tuxfisher_x+(2*x) >= 0)
				if (map[tuxfisher_x+(2*x)][tuxfisher_y]==CASE_VIDE)
				{
					addPreviousMap();
					map[tuxfisher_x+(2*x)][tuxfisher_y]=CASE_AVALANCHE;
					map[tuxfisher_x+x][tuxfisher_y]=CASE_TUXFISHER;
					map[tuxfisher_x][tuxfisher_y]=CASE_VIDE;
					dernier_mouvement=deplacement;
					tuxfisher_x+=x;
					tuxfisher_y+=y;
					retour = RET_NEIGE; // le retour par défaut change car la boule est poussée
			}
			return retour;
	}

    return retour;
}

bool GameEngine::dropAvalanche()
{
    bool continuer=true;
    int priorite_x;
    if (avalanche_x<COORD_X && avalanche_y+1<COORD_Y && avalanche_x>= NULL && avalanche_y+1>= NULL)
    {
        switch(map[avalanche_x][avalanche_y+1])
        {
            case ERREUR:
            				break;
            case CASE_VIDE:
                            map[avalanche_x][avalanche_y]=CASE_VIDE;
                            avalanche_y+=1;
                            map[avalanche_x][avalanche_y]=CASE_AVALANCHE;
                            break;
            case CASE_TUXFISHER:
                            game_over=true;
                            continuer=false;
                            break;
            case CASE_MONTAGNE:
            case CASE_POISSON:
            case CASE_NEIGE:
                            continuer=false;
                            break;
            case CASE_AVALANCHE:
                            if (dernier_mouvement==MVT_GAUCHE)
                                priorite_x=-1;
                            else
                                priorite_x=1;
			    if (avalanche_x+priorite_x<COORD_X && avalanche_x+priorite_x>= NULL)
                            {
                            	if (map[avalanche_x+priorite_x][avalanche_y]==CASE_VIDE &&
                                	map[avalanche_x+priorite_x][avalanche_y+1]==CASE_VIDE)
                                {
                                    map[avalanche_x][avalanche_y]=CASE_VIDE;
                                    avalanche_x+=priorite_x;
                                    map[avalanche_x][avalanche_y]=CASE_AVALANCHE;
                                    break;
                                }
								if (map[avalanche_x+priorite_x][avalanche_y]==CASE_VIDE &&
                                	map[avalanche_x+priorite_x][avalanche_y+1]==CASE_TUXFISHER)
                                { // TUXFISHER vas mourir
                                    map[avalanche_x][avalanche_y]=CASE_VIDE;
                                    avalanche_x+=priorite_x;
                                    map[avalanche_x][avalanche_y]=CASE_AVALANCHE;
                                    break;
                                }
                            }
                            priorite_x*=-1;
			    if (avalanche_x+priorite_x<COORD_X && avalanche_x+priorite_x>= NULL)
                            {
                            	if (map[avalanche_x+priorite_x][avalanche_y]==CASE_VIDE &&
                                	map[avalanche_x+priorite_x][avalanche_y+1]==CASE_VIDE)
                                {
                                    map[avalanche_x][avalanche_y]=CASE_VIDE;
                                    avalanche_x+=priorite_x;
                                    map[avalanche_x][avalanche_y]=CASE_AVALANCHE;
                                    break;
                                }
								if (map[avalanche_x+priorite_x][avalanche_y]==CASE_VIDE &&
                                	map[avalanche_x+priorite_x][avalanche_y+1]==CASE_TUXFISHER)
                                { // TuxFisher vas mourir
                                    map[avalanche_x][avalanche_y]=CASE_VIDE;
                                    avalanche_x+=priorite_x;
                                    map[avalanche_x][avalanche_y]=CASE_AVALANCHE;
                                    break;
                                }
                            }
							continuer=false;
                            break;
        }
    }
    else
		continuer=false;
	return continuer;
}

bool GameEngine::fallingAvalanche()
{
    for (int countx=COORD_X-1; countx>= NULL ; countx--)
	for (int county=COORD_Y-1 ; county>= NULL ; county--)
            if ((map[countx][county]==CASE_AVALANCHE) && (county+1<COORD_Y))
                if (map[countx][county+1]==CASE_VIDE)
                {
                    avalanche_x = countx;
                    avalanche_y = county;
                    return true;
                }
    return false;
}
