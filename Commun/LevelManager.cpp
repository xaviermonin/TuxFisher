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

#include <QtGui>
#include <iostream>
#include "LevelManager.h"
#include "definitions.h"

LevelManager::LevelManager()
{
	numeromap = -1; // Level actuel (commence à partir de 0)
	version = 0;
}

bool LevelManager::save()
{
	QFile file(getFileName());
	QDateTime date = QDateTime::currentDateTime();
	QByteArray bytearray_num_version;
	if (file.open(QFile::WriteOnly))
	{
		char charact_difficulty;
		char charact_x;
		char charact_y;
		char charact_nb;
		file.write("[TuxFisher]");
		file.write(MAP_VERSION.toLatin1().data());
		std::cerr << "Numero version: " << numeroversion;
		file.write((char*)&numeroversion, 2);
		file.write(nomlevel.toLatin1()+"\n");
		file.write(nomauthor.toLatin1()+"\n");
		charact_difficulty=difficulty_level;
		charact_nb=nommap.size();
		file.write(&charact_difficulty,1);
		file.write(&charact_nb,1);
		
		for (int count=0; count<charact_nb; count++)
		{
			charact_x=sizemap.at(count).width();
			charact_y=sizemap.at(count).height();
			file.write(&charact_x,1);
			file.write(&charact_y,1);
			file.write(nommap.at(count).toLatin1()+"\n");
			file.write(datas.at(count));
		}
		file.close();
	}
	else
		return false;
	return true;
}

int LevelManager::getMapsDebloquees()
{
	QFile fileinfos(dir_name+filenameinfosmaps);
	if (fileinfos.open(QIODevice::ReadWrite))
	{
		while(!fileinfos.atEnd())
		{
			QString string=fileinfos.readLine().simplified();
			QStringList stringlist=string.split(",");
			if (stringlist.count()==2)
				if (stringlist[0]==getFileName())
					return stringlist[1].toInt();
		}
	}
	return 1;
}

void LevelManager::saveMapsDebloquees(int mapsunlocked)
{
	QFile file_lecture(dir_name+filenameinfosmaps);
	QStringList infosnomsfiles;
	QList<int> infosmapsunlocked;

	// On stock toutes les infos des maps
	if (file_lecture.open(QIODevice::ReadWrite))
	{
		while(!file_lecture.atEnd())
		{
			QString string=file_lecture.readLine().simplified();
			QStringList stringlist=string.split(",");
			if (stringlist.count()==2)
				if (stringlist[0]!=getFileName())
				{
					infosnomsfiles << stringlist[0];
					infosmapsunlocked << stringlist[1].toInt();
				}
		}
		file_lecture.close();
	}

	// On enregistre tout
	QFile file_ecriture(dir_name+filenameinfosmaps);
	if (file_ecriture.open(QIODevice::WriteOnly))
	{
		QString string_mapsunlocked;
		string_mapsunlocked.setNum(mapsunlocked);
		QString string(getFileName()+","+string_mapsunlocked+"\r\n");
		file_ecriture.write(string.toLatin1().data());
		for (int count=0; count<infosnomsfiles.count(); count++)
		{
			string_mapsunlocked.setNum(infosmapsunlocked[count]);
			QString string(infosnomsfiles[count]+","+string_mapsunlocked+"\r\n");
			file_ecriture.write(string.toLatin1().data());
		}
		file_ecriture.close();
	}
}

bool LevelManager::newLevel(QString level, QString author)
{
	numeromap= -1;
	nomlevel = level;
	nomauthor = author;
	nommap.clear();
	datas.clear();
	sizemap.clear();
	numeroversion = 0;
	return true;
}

bool LevelManager::newMap(QString map, QSize size, QByteArray datasmap)
{
	if (map.isEmpty())
		return false;
	if (size.width()*size.height() != datasmap.size())
		return false;
	nommap << map;
	sizemap << size;
	datas << datasmap;
	numeromap=nommap.size()-1;
	return true;
}

bool LevelManager::loadFile(QString file)
{
	QFile filelevel(file);
	if (!filelevel.open(QIODevice::ReadOnly))
	{
		error = QObject::tr("Le file %1 n'a pas été ouvert.").arg(file);
		return false;
	}
	int retour = loadFile(filelevel);
	
	filelevel.close();
	return retour;
}

bool LevelManager::loadFile(QString level, QString author)
{
	QString filename=QString((dir_name+level+"-"+author+".tf").toLower());
	int retour = loadFile(filename);
	
	return retour;
}

bool LevelManager::loadFile(QIODevice &filelevel)
{
	if (!filelevel.isOpen())
	{
		error = QObject::tr("Le file du jeu n'a pas été ouvert.");
		return false;
	}
	int nbmap;
	char charact;
	QByteArray controle;
	
	numeromap=-1;
	nomlevel.clear();
	nommap.clear();
	datas.clear();
	sizemap.clear();
	
	filelevel.seek(0);
	
	QString chaineclef = filelevel.read(11);
	if (chaineclef=="[TuxFisher]")
	{
		// Lit la version du level
		QString version = filelevel.read(8);
		if (version!=MAP_VERSION)
		{
			error = QObject::tr("La version de la partie n'est pas supportée.");
			return false;
		}
		// Lit le numero de version
		filelevel.read((char*) &numeroversion, 2);
		// Lit le nom du level
		nomlevel = filelevel.readLine().simplified();
		if (nomlevel.isEmpty())
		{
			error = QObject::tr("La partie n'a pas de nom.");
			return false;
		}
		// Lit le nom de l'author
		nomauthor = filelevel.readLine().simplified();
		if (nomauthor.isEmpty())
		{
			error = QObject::tr("La partie n'a pas d'author.");
			return false;
		}
		// Lit le level de difficulté du level
		filelevel.read(&charact, 1);
		difficulty_level = (NIVEAU)charact;
		// Lit le nombre de maps du level
		filelevel.read(&charact, 1);
		nbmap = charact;
		// - Partie qui lit les noms de map du level -
		for (int c=0; c<nbmap; c++)
		{
			QSize size;
			// Lit la size d'une map
			filelevel.read(&charact, 1);
			size.setWidth(charact);
			filelevel.read(&charact, 1);
			size.setHeight(charact);
			sizemap << size;
			int nboctets = size.width()*size.height();
			// Lit le nom d'une map
			nommap << filelevel.readLine().simplified();
			// Lit les données de la map
			QByteArray octets(filelevel.read(nboctets));
			if (octets.size()!=nboctets)
			{
				error = QObject::tr("Les levelx du jeu n'ont pas pu être lu correctement.");
				return false;
			}
			datas << octets;
		}
		if (nbmap != nommap.size())
		{
			error = QObject::tr("Les level lus ne correspondent pas au nombre de level présents");
			return false;
		}
	}
	else
	{
		error = QObject::tr("Le file n'est pas un partie %1.").arg(APPLICATION_NAME);
		return false;
	}
	numeromap=0; // = 1 map
	return true;
}

bool LevelManager::removeMap()
{
	if (nommap.size()>1) // Si plus d'une map existe
	{
		nommap.removeAt(numeromap);
		datas.removeAt(numeromap);
		sizemap.removeAt(numeromap);
		if (numeromap>0)
		{
			numeromap--;
			selectMap(numeromap);
		}
		else
		{
			numeromap = 0;
			selectMap(numeromap);
		}
		return true;
	}
	else
		return false; // Pas assez de map, donc rien à remove
}

bool LevelManager::selectMap(int num)
{
	if (nommap.size()>num && num>=0)
	{
		numeromap = num;
		return true;
	}
	else
		return false;
}

bool LevelManager::loadMap(QByteArray map)
{
	if (datas.size()>=numeromap)
	{
		datas.replace(numeromap, map);
		return true;
	}
	return false;
}

bool LevelManager::loadMap(QByteArray map, QSize size)
{
	if (datas.size()>=numeromap)
	{
		if (size.width()*size.height() != map.size())
			return false;
		datas.replace(numeromap, map);
		sizemap.replace(numeromap, size);
		return true;
	}
	return false;
}

bool LevelManager::setNameMap(QString nom)
{
	if (nommap.size()>=numeromap)
		if (!nom.isEmpty())
		{
			nommap.replace(numeromap, nom);
			return true;
		}
	return false;
}

bool LevelManager::setNameLevel(QString nom)
{
	if (!nom.isEmpty())
	{
		nomlevel = nom;
		return true;
	}
	return false;
}

bool LevelManager::setNameAuthor(QString nom)
{
	if (!nom.isEmpty())
	{
		nomauthor = nom;
		return true;
	}
	return false;
}

bool LevelManager::setDifficulte(NIVEAU niv)
{
	difficulty_level=niv;
	return true;
}

bool LevelManager::setMaps(int premier, int second)
{
	int indexselection=numeromap; // Map sélectionnée

	if (premier>=nommap.size() && premier<0)
		return false;
	if (second>=nommap.size() && second<0)
		return false;

	QString string_temp=nommap.at(premier);
	QSize size_temp=sizemap.at(premier);
	QByteArray bytearray_temp=datas.at(premier);

	nommap[premier]=nommap[second];
	sizemap[premier]=sizemap[second];
	datas[premier]=datas[second];

	nommap[second]=string_temp;
	sizemap[second]=size_temp;
	datas[second]=bytearray_temp;

	selectMap(indexselection); // On revient à la map selectionnée avant

	return true;
}

QByteArray LevelManager::getDatasMap()
{
	if (datas.size()<numeromap)
		return datas.at(0);
	else
		return datas.at(numeromap);
}

QSize LevelManager::getSizeMap()
{
	if (sizemap.size()<numeromap)
		return sizemap.at(0);
	else
		return sizemap.at(numeromap);		
}

QString LevelManager::getNameMap()
{
	if (nommap.size()<numeromap)
		return "";
	else
		return nommap.at(numeromap);
}

// Fonction statique renvoyant les informations de tous les levelx du répertoire maps
void LevelManager::getInfosLevelx(QStringList &list_namefile,
	QStringList &list_namelevel, QList<NIVEAU> &list_difficulty, QStringList &list_nameauthor,
	QList<int> &list_mapsunlocked, QList<QStringList> &list_namemap)
{
	QDir repertoire(dir_name);
	QStringList list_repertoire = repertoire.entryList((QStringList)"*.tf", QDir::Files);
	LevelManager gestion;
	for (int i = 0; i < list_repertoire.size(); ++i)
	{
		QString file(dir_name+(QString)QDir::separator()+list_repertoire.at(i));
		QStringList nommaps;
		if(gestion.loadFile(file))
		{
			list_namefile << file;
			list_namemap << gestion.getListNameMap();
			list_namelevel << gestion.getNameLevel();
			list_difficulty << gestion.getDifficulty();
			list_mapsunlocked << gestion.getMapsDebloquees();
			list_nameauthor << gestion.getNameAuthor();
		}
	}
}

QString LevelManager::dir_name="maps"+(QString)QDir::separator();
QString LevelManager::filenameinfosmaps="maps.cfg";
