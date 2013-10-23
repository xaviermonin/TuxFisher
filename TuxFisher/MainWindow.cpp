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
#include <QtGui>
#include "SoundManager.h"
#include "MainWindow.h"
#include "GameWidget.h"
#include "../Commun/ChoiceLevelDialog.h"
#include "LevelDownloaderDialog.h"
#include "../Commun/LevelManager.h"
#include "DidYouKnowDialog.h"
#include "../Commun/definitions.h"

MainWindow::MainWindow()
{
	QCoreApplication::setOrganizationName("Xavier Monin");
	QCoreApplication::setOrganizationDomain("tuxfisher.net");
	QCoreApplication::setApplicationName(APPLICATION_NAME);

	QDir::setCurrent(QCoreApplication::applicationDirPath());
	setWindowIcon(QIcon(":/ressources/mainwindow/logo.png"));

	audio = SoundManager::getInstance();
	WTuxFisher = new GameWidget(this);
	level_tuxfisher = new LevelManager;
	
	setCentralWidget(WTuxFisher);

	switchWindowTitle(false);
	
	no_sound = tr("Aucun son");
	default_graphism = tr("Classique");

	createActions();
	createMenus();
	createToolBar();
	createDocks();
	createStateBar();

	did_u_know_dialog = new DidYouKnowDialog(this);
	
	// Créations et configurations des timers
	timer_time = new QTimer(this);
	timer_gameover = new QTimer(this);
	timer_gameover->setInterval(2000);
	timer_gameover->setSingleShot(true);
	timer_level_completed = new QTimer(this);
	timer_level_completed->setInterval(2000);
	timer_level_completed->setSingleShot(true);

	pixmap_reload = QPixmap(":/ressources/mainwindow/reload.png");
	nb_moves=0;
	sec_elapsed=0;
	
	connect(WTuxFisher, SIGNAL(gameOver()), this, SLOT(gameOver()));
	connect(WTuxFisher, SIGNAL(levelCompleted()), this, SLOT(levelCompleted()));
	connect(WTuxFisher, SIGNAL(addMove(bool)), this, SLOT(addMove(bool)));

	connect(timer_time, SIGNAL(timeout()), this, SLOT(updateTimeElapsed()));
	connect(timer_gameover, SIGNAL(timeout()), this, SLOT(loadMap()));
	connect(timer_level_completed, SIGNAL(timeout()), this, SLOT(nextMap()));

	connect(menu_map, SIGNAL(triggered(QAction *)), this, SLOT(setMap(QAction *)));
	
	loadConfiguration();
	setStatBarMsg(tr("%1 est chargé").arg(APPLICATION_NAME));
}

MainWindow::~MainWindow()
{
	SoundManager::kill();
	delete level_tuxfisher;
	delete actions_graphism;
	delete actions_sound;
}

void MainWindow::updateTimeElapsed()
{
	if (!isActiveWindow())
		return;
	sec_elapsed+=1;
	QTime time;
	time= time.addSecs(sec_elapsed);
	QString temps = time.toString("mm:ss");
	lcd_time_elapsed->display(temps);
}

QIcon MainWindow::createIcon(int i)
{
	QPixmap pixmap = pixmap_reload;
	QPainter painter(&pixmap);
	painter.setFont(QFont("Times", 18, QFont::Bold));
	painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::number(i+1));
	painter.end();
	return QIcon(pixmap);
}

void MainWindow::createActions()
{
	action_load_file = new QAction(tr("&Charger Partie..."), this);
	action_load_file->setShortcut(tr("CTRL+C"));
	action_load_file->setIcon(QIcon(":/ressources/mainwindow/load.png"));
	connect(action_load_file, SIGNAL(triggered()),
			this, SLOT(loadLevel()));
	
	action_download_level = new QAction(tr("&Téléload partie..."), this);
	action_download_level->setShortcut(tr("CTRL+T"));
	action_download_level->setIcon(QIcon(":/ressources/mainwindow/teleload.png"));
	connect(action_download_level, SIGNAL(triggered()),
			this, SLOT(downloadLevel()));
	
	action_quit = new QAction(tr("&Quitter"), this);
	action_quit->setShortcut(tr("CTRL+Q"));
	action_quit->setIcon(QIcon(":/ressources/mainwindow/quitter.png"));
	connect(action_quit, SIGNAL(triggered()),
			this, SLOT(close()));
	
	action_help = new QAction(tr("Aide"), this);
	action_help->setShortcut(tr("F1"));
	action_help->setIcon(QIcon(":/ressources/mainwindow/help.png"));
	connect(action_help, SIGNAL(triggered()),
			this, SLOT(help()));

	action_did_u_know = new QAction(tr("&Le saviez vous?"), this);
	action_did_u_know->setShortcut(tr("CTRL+L"));
	action_did_u_know->setIcon(QIcon(":/ressources/mainwindow/saviezvous.png"));
	connect(action_did_u_know, SIGNAL(triggered()),
			this, SLOT(didYouKnow()));
	
	action_about = new QAction(tr("A &Propos de %1").arg(APPLICATION_NAME), this);
	action_about->setShortcut(tr("CTRL+P"));
	action_about->setIcon(QIcon(":/ressources/mainwindow/logo.png"));
	connect(action_about, SIGNAL(triggered()),
			this, SLOT(about()));
	
	// Partie Map
	// Action choix map
	action_map = new QAction(tr("Level..."),this);
	menu_map = new QMenu(tr("Level"),this);
	action_map->setMenu(menu_map);
	connect(action_map, SIGNAL(triggered()),
			this, SLOT(loadMap()));

	// Actions map nexte et précedente
	action_map_previous = new QAction(tr("Level précédent"),this);
	action_map_previous->setIcon(QIcon(":/ressources/mainwindow/previous.png"));
	connect(action_map_previous, SIGNAL(triggered()),
			this, SLOT(previousMap()));
	
	action_map_next = new QAction(tr("Level next"),this);
	action_map_next->setIcon(QIcon(":/ressources/mainwindow/next.png"));
	connect(action_map_next, SIGNAL(triggered()),
			this, SLOT(nextMap()));

	// Partie Options
	// Actions graphisme
	actions_graphism = new QList<QAction *>;
	QStringList listgraphisme = getGraphism();
	for (int i=0; i<listgraphisme.size(); ++i)
	{
		actions_graphism->append(new QAction(listgraphisme.at(i),this));
		actions_graphism->at(i)->setCheckable(true);
	}
	
	actiongroup_graphism = new QActionGroup(this);
	for (int i=0; i<actions_graphism->size(); ++i)
		actiongroup_graphism->addAction(actions_graphism->at(i));
	
	// Action music
	action_music = new QAction(tr("Musique"), this);
	action_music->setCheckable(true);
	connect(action_music, SIGNAL(toggled(bool)),
			audio, SLOT(jouerMusique(bool)));
	
	// Actions son
	actions_sound = new QList<QAction *>;
	QStringList listson = getSound();
	for (int i=0; i<listson.size(); ++i)
	{
		actions_sound->append(new QAction(listson.at(i),this));
		actions_sound->at(i)->setCheckable(true);
	}
	
	actiongroup_sound = new QActionGroup(this);
	for (int i=0; i<actions_sound->size(); ++i)
		actiongroup_sound->addAction(actions_sound->at(i));

	connect(actiongroup_graphism, SIGNAL(triggered(QAction*)),
			this, SLOT(setGraphism(QAction*)));
	connect(actiongroup_sound, SIGNAL(triggered(QAction*)),
			this, SLOT(setSound(QAction*)));
}

void MainWindow::loadMap()
{
	timer_gameover->stop();
	if (!level_tuxfisher->selectMap(level_tuxfisher->getNumeroMap()))
		return;
	QByteArray datas;
	QSize size;
	datas = level_tuxfisher->getDatasMap();
	size = level_tuxfisher->getSizeMap();
	timer_time->stop();
	lcd_time_elapsed->display("00:00");
	lcd_moves->display("0");
	sec_elapsed=0;
	nb_moves=0;
	WTuxFisher->loadMap(size.width(), size.height(), datas);
	updateMapMenu();
	setStatBarMsg(tr("Chargement de la partie terminée."));
}

void MainWindow::switchWindowTitle(bool level)
{
	if (level)
		setWindowTitle(tr("%1 - %2 de %3").arg(APPLICATION_NAME).arg(level_tuxfisher->getNameLevel()).arg(level_tuxfisher->getNameAuthor()));
	else
		setWindowTitle(APPLICATION_NAME);
}

void MainWindow::loadConfiguration()
{
	QSettings configuration("Monin Xavier", APPLICATION_NAME);
	bool premierlancement = configuration.value("premierlancement", true).toBool();
	bool saviezvous = configuration.value("saviezvous", true).toBool();
	bool music = configuration.value("music", false).toBool();
	QString stringSon = configuration.value("son").toString();
	QString stringGraphism = configuration.value("graphisme", default_graphism).toString();
	
	bool ok=false;

	for (int count=0; count<actions_sound->count(); count++)
		if (actions_sound->at(count)->text()==stringSon)
		{
			actions_sound->at(count)->trigger(); // Si on trouve le son enregistré on le sélectionne
			ok=true;
			break;
		}

	if (!ok && actions_sound->size()>0) // Si on a rien trouvé
		if (actions_sound->at(0)->text()==no_sound)
		{
			if (actions_sound->size()>1)
				actions_sound->at(1)->trigger(); // On sélectionne le premier vrai son
			else
				actions_sound->at(0)->trigger(); // On sélectionne le choix aucunson si il est le seul disponible
		}
		else
			actions_sound->at(0)->trigger(); // On selectionne le premier vrai son

	ok=false;

	for (int count=0; count<actions_graphism->count(); count++)
		if (actions_graphism->at(count)->text()==stringGraphism)
		{
			actions_graphism->at(count)->trigger();
			ok=true;
			break;
		}
	if (!ok)
		actions_graphism->at(0)->trigger();

	action_music->setChecked(music);
	if (saviezvous)
	{
		if (premierlancement)
			did_u_know_dialog->displayWelcome();

		did_u_know_dialog->setCheckBox(false);
		did_u_know_dialog->show();
	}
	else
		did_u_know_dialog->setCheckBox(true);
}

void MainWindow::writeConfiguration()
{
	QSettings configuration("Monin Xavier", QString("%1").arg(APPLICATION_NAME));
	configuration.setValue("premierlancement" , false);
	configuration.setValue("music" , action_music->isChecked());
	configuration.setValue("saviezvous" , did_u_know_dialog->displayed());
	configuration.setValue("son" , actiongroup_sound->checkedAction()->text());
	configuration.setValue("graphisme" , actiongroup_graphism->checkedAction()->text());
}

void MainWindow::addMove(bool ajouter)
{
	if (!timer_time->isActive() && ajouter)
		timer_time->start(1000);
	if (ajouter)
		nb_moves++;
	if (!ajouter && nb_moves>=0)
		nb_moves--;
	lcd_moves->display(QString::number(nb_moves));
}

void MainWindow::updateMapMenu()
{
	menu_map->setEnabled(false);
	menu_map->clear();
	actions_map.clear();

	QStringList list_map=level_tuxfisher->getListNameMap();
	int mapsunlocked=level_tuxfisher->getMapsDebloquees();
	int nbmaps=list_map.count();

	for (int i=0; i<nbmaps; ++i)
	{
		if (i>=mapsunlocked)
				list_map[i]=tr("Level non débloqué");
		QAction *action = new QAction(list_map.at(i), menu_map);
		if (i>=mapsunlocked)
			action->setDisabled(true);
		action->setIcon(createIcon(i));
		actions_map << action;
	}

	int mapselectionnee=level_tuxfisher->getNumeroMap();

	if (mapselectionnee<=0)
		action_map_previous->setDisabled(true);
	else
		action_map_previous->setDisabled(false);

	if (mapselectionnee>=nbmaps-1 || mapselectionnee>=mapsunlocked-1)
		action_map_next->setDisabled(true);
	else
		action_map_next->setDisabled(false);
	
	menu_map->addActions(actions_map);
	menu_map->setEnabled(true);
	std::cerr << "BUG? ";
}

void MainWindow::setGraphism(QAction *action)
{
	WTuxFisher->loadGraphism(action->text());
}

void MainWindow::setSound(QAction *action)
{
	WTuxFisher->loadSound(action->text());
}

void MainWindow::setMap(QAction *action)
{
	QStringList list_map=level_tuxfisher->getListNameMap();
	for (int i=0; i<list_map.count(); ++i)
		if (list_map.at(i) == action->text())
		{
			level_tuxfisher->selectMap(i);
			action_map->setIcon(createIcon(i));
			loadMap();
			break;
		}
}

QStringList MainWindow::getGraphism()
{
	QString dir_name("graphismes"+(QString)QDir::separator());
	QDir repertoire(dir_name);
	QStringList list_repertoire;
	list_repertoire << default_graphism; // Graphism par défaut
	list_repertoire += repertoire.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int i=1; i<list_repertoire.size(); ++i)
	{
		if (!QFileInfo(dir_name+list_repertoire.at(i)+(QString)QDir::separator()+"avalanche.png").isFile())
		{
			list_repertoire.removeAt(i);
			i--;
		}
	}
	return list_repertoire;
}

QStringList MainWindow::getSound()
{
	QString dir_name("sons"+(QString)QDir::separator());
	QDir repertoire(dir_name);
	QStringList list_repertoire;
	list_repertoire << no_sound;
	list_repertoire += repertoire.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int i=1; i<list_repertoire.size(); ++i)
	{
		if (!QFileInfo(dir_name+list_repertoire.at(i)+(QString)QDir::separator()+"avalanche.wav").isFile())
		{
			list_repertoire.removeAt(i);
			i--;
		}
	}
	return list_repertoire;
}

void MainWindow::createMenus()
{
	menu_file = menuBar()->addMenu(tr("&Fichier"));
	menu_file->addAction(action_load_file);
	menu_file->addAction(action_download_level);
	menu_file->addSeparator();
	menu_file->addAction(action_quit);
	
	menu_options = menuBar()->addMenu(tr("&Options"));
	menu_options->addAction(action_music);
	menu_options_graphism = new QMenu(tr("Graphisms"),this);
	menu_options_graphism->setIcon(QIcon(":/ressources/mainwindow/graphisme.png"));
	menu_options_graphism->addActions(actiongroup_graphism->actions());
	menu_options->addMenu(menu_options_graphism);
	
	menu_options_son = new QMenu(tr("Sons"),this);
	menu_options_son->setIcon(QIcon(":/ressources/mainwindow/son.png"));
	menu_options_son->addActions(actiongroup_sound->actions());
	menu_options->addMenu(menu_options_son);
	
	menuBar()->addSeparator();
	
	menu_help = menuBar()->addMenu(tr("&Aide"));
	menu_help->addAction(action_help);
	menu_help->addAction(action_did_u_know);
	menu_help->addAction(action_about);
}

void MainWindow::createToolBar()
{
	toolbar_file = addToolBar(tr("&Fichier"));
	toolbar_file->setAllowedAreas(Qt::BottomToolBarArea);
	toolbar_file->addAction(action_load_file);
	toolbar_file->setMovable(false);

	toolbar_map = addToolBar(tr("&Levelx"));
	toolbar_map->addAction(action_map_previous);
	toolbar_map->addAction(action_map);
	toolbar_map->addAction(action_map_next);
	toolbar_map->setMovable(false);
	toolbar_map->setVisible(false);

	toolbar_time_elapsed = addToolBar(tr("Temp&s écoulé"));

	lcd_time_elapsed = new QLCDNumber(this);
	lcd_time_elapsed->setSegmentStyle(QLCDNumber::Filled);
	lcd_time_elapsed->display("00:00");
	lcd_moves = new QLCDNumber(this);
	lcd_moves->setSegmentStyle(QLCDNumber::Flat);
	lcd_moves->display("0");

	toolbar_time_elapsed->addWidget(lcd_time_elapsed);
	toolbar_time_elapsed->addWidget(lcd_moves);
	toolbar_time_elapsed->setMovable(false);
	toolbar_time_elapsed->setVisible(false);

}

void MainWindow::createDocks()
{
	
}

void MainWindow::createStateBar()
{
	msgstatbar = new QLabel(this);
	msgstatbar->setIndent(3);
	
	statusBar()->addWidget(msgstatbar,1);
	statusBar()->setSizeGripEnabled(false);
}

void MainWindow::setStatBarMsg(QString msg)
{
	msgstatbar->setText(msg);
}

void MainWindow::previousMap()
{
	int num = level_tuxfisher->getNumeroMap();
	num--;
	if (level_tuxfisher->selectMap(num))
	{
		loadMap();
		action_map->setIcon(createIcon(num));
	}
}

void MainWindow::nextMap()
{
	timer_level_completed->stop();
	int num = level_tuxfisher->getNumeroMap();
	num++;
	if (level_tuxfisher->selectMap(num))
	{
		loadMap();
		action_map->setIcon(createIcon(num));
	}
}

void MainWindow::gameOver()
{
	timer_time->stop();
	setStatBarMsg(tr("Partie perdue."));
	timer_gameover->start();
}

void MainWindow::levelCompleted()
{
	timer_time->stop();
	level_tuxfisher->saveMapsDebloquees(level_tuxfisher->getNumeroMap()+2);
	updateMapMenu();
	setStatBarMsg(tr("Partie gagnée."));
	timer_level_completed->start();
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("A Propos de %1").arg(APPLICATION_NAME),
		tr("<p>%1 - Licence GPL<br/>Copyright &copy; 2010 - Xavier Monin<br/><br/>"
		"Développeur: Xavier Monin<br />"
		"Site Web: <a href=\"http://wwww.tuxfisher.net/\">http://wwww.tuxfisher.net</a><br />"
		"Musique: General Fuzz - Acclimate <a href=\"http://creativecommons.org/licenses/by-nc-nd/2.0/deed.fr\">(Licence Creative Common)</a>").arg(APPLICATION_NAME));
}

void MainWindow::loadLevel()
{
	ChoiceLevelDialog choixleveldialog(this);
	if (choixleveldialog.exec())
	{
		QString file;
		int numeromap;
		file=choixleveldialog.getFileName();
		numeromap = choixleveldialog.getNumeroMap();
		setStatBarMsg(tr("Chargement en cours..."));
		if (!level_tuxfisher->loadFile(file))
		{
			setStatBarMsg(tr("Chargement échoué."));
			QMessageBox::information(this,tr("Chargement de la partie impossible"),
				tr("La partie n'a pas pu être chargée"), QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		level_tuxfisher->selectMap(numeromap);
		loadMap();
		action_map->setIcon(createIcon(numeromap));
		toolbar_map->setVisible(true);
		toolbar_time_elapsed->setVisible(true);

		switchWindowTitle(true);

		setStatBarMsg(tr("Chargement de la partie terminé."));
		
		if (!isMaximized())
			adjustSize();
	}
}

void MainWindow::downloadLevel()
{
	LevelDownloaderDialog download_leveldialog(this);
	download_leveldialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	int reponse=QMessageBox::warning(this, APPLICATION_NAME,
                        tr("Etes vous sûr de vouloir quitter %1?").arg(APPLICATION_NAME),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No);
	if (reponse==QMessageBox::Yes)
	{
		writeConfiguration();
		event->accept();
	}
	else
		event->ignore();
}
