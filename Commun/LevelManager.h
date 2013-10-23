/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
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

#ifndef _LEVEL_MANAGER_H_
#define _LEVEL_MANAGER_H_

#include <QList>
class QString;
class QSize;
class QFile;
class QByteArray;

// List des levelx de difficulté disponibles
typedef enum { NIV_TOUS=0, NIV_FACILE=1, NIV_MOYEN=2, NIV_DIFFICILE=3 } NIVEAU;

class LevelManager
{
public:
	LevelManager();
	bool newLevel(QString, QString);
	bool newMap(QString, QSize, QByteArray);
	bool loadFile(QString);
	bool loadFile(QIODevice &);
	bool loadFile(QString, QString);
	bool loadMap(QByteArray, QSize);
	bool loadMap(QByteArray);
	bool removeMap();
	bool selectMap(int);
	QSize getSizeMap();
	QByteArray getDatasMap();
	bool setMaps(int, int);
	inline int getNumeroMap() { return numeromap; };
	inline QString getNameLevel() { return nomlevel; }
	inline QString getNameAuthor() { return nomauthor; }
	inline QString getFileName() { return QString(dir_name+nomlevel+"-"+nomauthor+".tf").toLower(); }
	inline QString getError() { QString error_courante=error; error=""; return error_courante; }
	inline NIVEAU getDifficulty() { return difficulty_level; };
	inline QStringList getListNameMap() { return nommap; };
	inline qint16 getNumeroVersion() { return numeroversion; };
	inline void updateVersion() { numeroversion+=1; };
	int getMapsDebloquees();
	void saveMapsDebloquees(int);
	QString getNameMap();
	//inline QStringList getMapList() { return datas; }
	bool setNameLevel(QString);
	bool setNameAuthor(QString);
	bool setNameMap(QString);
	bool setDifficulte(NIVEAU);
	bool save();
	
	static void getInfosLevelx(QStringList &, QStringList &, QList<NIVEAU> &, QStringList &, QList<int> &, QList<QStringList> &);

private:
	int numeromap; // Index de la map actuelle
	qint16 numeroversion;
	QString nomlevel;
	QString nomauthor;
	static QString dir_name; // Répertoire des maps
	static QString filenameinfosmaps; // Name du file qui contient les infos (si un joueur a terminé tel map)
	char version;
	QString error;
	NIVEAU difficulty_level;
	QStringList nommap;
	QList<QByteArray> datas;
	QList<QSize> sizemap;
};

#endif // !_GESTIONNIVEAU_TUXFISHER_H_
