/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de niveaux pour TuxFisher
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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QActionGroup>
#include <QPixmap>
#include <QLabel>
#include <QIcon>

class EditorWidget;
class LevelManager;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
protected:
	void closeEvent(QCloseEvent *event);
private:
	void createMenus();
	void createActions();
	void createToolBar();
	QIcon createIcon(int);
	void switchWindowTitle(bool);
	void activateButtons();
	void updateMapMenu();
	bool checkMap();
	
	EditorWidget *editor;
	LevelManager *level_tuxfisher;
	
	QMenu *menu_file;
	QMenu *menu_level;
	QMenu *menu_map;
	QMenu *menu_help;

	QMenu *menu_toolbar_map;
	
	QAction *action_newmap;
	QAction *action_quit;
	QAction *action_about;
	QAction *action_save;
	QAction *action_upload;
	QAction *action_load;
	
	QList<QAction *> actions_map;
	QAction *action_map;
	QAction *action_map_previous;
	QAction *action_map_next;

	QAction *action_level_infolevel;
	
	QAction *action_map_infomap;
	QAction *action_map_sortmaps;
	QAction *action_map_newmap;
	QAction *action_map_removemap;
	
	QAction *action_casemontagne;
	QAction *action_caseavalanche;
	QAction *action_casepoisson;
	QAction *action_casetuxfisher;
	QAction *action_casevide;
	QAction *action_caseneige;
	
	QActionGroup *groupaction;
	
	QPixmap pixmap_reload;
	
	QToolBar *toolbar_file;
	QToolBar *toolbar_map;
	
	QLabel *labelstartup;
	
	QString filename;
	
	QSize sizemap;
	
	bool modification;

private slots:
	void newLevel();
	void newMap();
	void setInfoLevel();
	void setInfoMap();
	void removeMap();
	void about();
	void setBrush(QAction *);
	void save();
	void upload();
	void load();
	void modifications() { modification=true; }
	void nextMap();
	void previousMap();
	void loadMap();
	void setMap(QAction *);
	void sortMaps();
};

#endif // !_MAINWINDOW_EDITEUR_H_
