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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include "DidYouKnowDialog.h"
#include "GameWidget.h"

class QAction;
class choixLevel_TuxFisher;
class QLabel;
class QActionGroup;
class SoundManager;
class LevelManager;
class QLCDNumber;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow();
	~MainWindow();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void keyPressEvent(QKeyEvent *) { WTuxFisher->setFocus(); };

private:
	void loadFile(QString);
	void createActions();
	void createMenus();
	void createToolBar();
	void createDocks();
	void createStateBar();
	void updateMapMenu();
	void switchWindowTitle(bool);
	void loadConfiguration();
	void writeConfiguration();
	QIcon createIcon(int);
	QStringList getGraphism();
	QStringList getSound();

	GameWidget *WTuxFisher;

	DidYouKnowDialog *did_u_know_dialog;

	QMenu *menu_file;
	QMenu *menu_options;
	QMenu *menu_options_graphism;
	QMenu *menu_options_son;
	QMenu *menu_help;
	QMenu *menu_map;
	
	QAction *action_load_file;
	QAction *action_download_level;
	QAction *action_quit;
	QAction *action_music;
	QList<QAction *> *actions_graphism;
	QList<QAction *> *actions_sound;
	QList<QAction *> actions_map;
	QAction *action_about;
	QAction *action_help;
	QAction *action_did_u_know;
	QAction *action_map;
	QAction *action_map_previous;
	QAction *action_map_next;
	
	QActionGroup *actiongroup_graphism;
	QActionGroup *actiongroup_sound;
	
	QPixmap pixmap_reload;
	
	QToolBar *toolbar_file;
	QToolBar *toolbar_map;
	QToolBar *toolbar_time_elapsed;

	QTimer *timer_time;
	QTimer *timer_gameover;
	QTimer *timer_level_completed;

	QLCDNumber *lcd_time_elapsed;
	QLCDNumber *lcd_moves;
	
	QLabel *msgstatbar;
	QLabel *label_startup;
	
	QString no_sound;
	QString default_graphism;
	
	SoundManager *audio;
	LevelManager *level_tuxfisher;
	
	int nb_moves;
	int sec_elapsed;

private slots:
	void levelCompleted(); // Level gagné
	void gameOver(); // Level perdu
	void setGraphism(QAction *);
	void setSound(QAction *);
	void setMap(QAction *);
	void loadMap();
	void previousMap();
	void nextMap();
	void downloadLevel();
	void addMove(bool);
	void updateTimeElapsed(); // Appelé par un timer. Mets à jour le compteur de temps
	void setStatBarMsg(QString);
	void loadLevel();
	void about();
	inline void help() { QDesktopServices::openUrl(QUrl("http://www.tuxfisher.net/categorie.comment-jouer.html")); };
	inline void didYouKnow() { did_u_know_dialog->show(); };
};


#endif // !_MAINWINDOW_TUXFISHER_H_
