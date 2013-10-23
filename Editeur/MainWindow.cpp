/*
Projet: Editeur TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 12/05/2008
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Editeur de levelx pour TuxFisher
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
#include "MainWindow.h"
#include "EditorWidget.h"
#include "../Commun/LevelManager.h"
#include "LevelDialog.h"
#include "MapDialog.h"
#include "LevelUploader.h"
#include "../Commun/ChoiceLevelDialog.h"
#include "MapSortDialog.h"
#include "../Commun/definitions.h"

MainWindow::MainWindow()
{
	filename="";
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	setWindowIcon(QIcon(":/ressources/mainwindow/logo.png"));
	modification=false;
	editor = new EditorWidget(this);
	level_tuxfisher = new LevelManager;
	pixmap_reload = QPixmap(":/ressources/mainwindow/reload.png");
	createActions();
	createMenus();
	createToolBar();
	switchWindowTitle(false);
	labelstartup = new QLabel;
	labelstartup->setPixmap(QPixmap(":/ressources/mainwindow/fond.png"));
	setCentralWidget(labelstartup);
	connect(editor, SIGNAL(modifications()), this, SLOT(modifications()));
	connect(menu_toolbar_map, SIGNAL(triggered(QAction *)), this, SLOT(setMap(QAction *)));
}

MainWindow::~MainWindow()
{
	delete level_tuxfisher;
}

void MainWindow::newLevel()
{
	filename="";
	LevelDialog dialog(this);
	if (dialog.exec())
	{
		QString nomlevel;
		QString nomauthor;
		
		// Chargement des données
		nomlevel = dialog.getName();
		nomauthor = dialog.getAuthor();
		
		// Création du level
		level_tuxfisher->newLevel(nomlevel, nomauthor);
		if (dialog.getDifficulty()==tr("Facile"))
			level_tuxfisher->setDifficulte(NIV_FACILE);
		else if (dialog.getDifficulty()==tr("Moyen"))
			level_tuxfisher->setDifficulte(NIV_MOYEN);
		else if (dialog.getDifficulty()==tr("Difficile"))
			level_tuxfisher->setDifficulte(NIV_DIFFICILE);
		else
			level_tuxfisher->setDifficulte(NIV_TOUS);
		
		// Création d'une map
		newMap();
		switchWindowTitle(true);
	}
}

void MainWindow::newMap()
{
	if (checkMap())
	{
		MapDialog dialoguemap(this);
		if (level_tuxfisher->getNumeroMap()>=0)
		{
			level_tuxfisher->loadMap(editor->getMap());
			dialoguemap.loadMapList(level_tuxfisher->getListNameMap());
		}
		if (dialoguemap.exec())
		{
			QString nommap;
			QByteArray map;
			sizemap = dialoguemap.getSize();
			nommap = dialoguemap.getName();

			editor->newMap(sizemap.width(), sizemap.height());
			map = editor->getMap();
			level_tuxfisher->newMap(nommap, sizemap, map);
			activateButtons();
			updateMapMenu();
			toolbar_map->setVisible(true);
			update();
			modification=true;
		}
	}
}

void MainWindow::sortMaps()
{
	level_tuxfisher->loadMap(editor->getMap());
	MapSortDialog sortmaps(this);
	sortmaps.loadStringList(level_tuxfisher->getListNameMap());
	if (sortmaps.exec())
	{
		QMap <int, int> sort = sortmaps.getSort();

		QMap <int, int> sort_en_cours; // On crée un sort qui servira de controle
		for (int count=0; count<sort.count(); count++)
			sort_en_cours[count]=count;
		
		for (int count=0; count<sort.count(); count++)
		{
			if (sort[count]!=sort_en_cours[count]) // On ne remplace pas si c'est déja bon
			{
				int i; // Stock les coordonnées de la cible (pour le remplacement)
				modification=true;
				for(i=0; i<sort_en_cours.count(); ++i)
					if (sort_en_cours[i]==sort[count]) // On sait maintenant a quel endroit est la cible
						break;

				level_tuxfisher->setMaps(count,i); // On échange donc les deux, la coordonnée count sera la bonne map!

				int temp=sort_en_cours[count]; // On met a jour le sort puisqu'il vient de set,
				sort_en_cours[count]=sort_en_cours[i]; // les coordonnées des cibles ont changés puisqu'on a éset des cibles
				sort_en_cours[i]=temp;
			}
		}
		updateMapMenu();
		loadMap();
	}
}

void MainWindow::setInfoLevel()
{
	LevelDialog dialoguelevel(this);
	dialoguelevel.loadLevelDialog(level_tuxfisher->getNameLevel(), level_tuxfisher->getNameAuthor(), level_tuxfisher->getDifficulty());
	if (dialoguelevel.exec())
	{
		// Chargement des données
		QString nomlevel = dialoguelevel.getName();
		QString nomauthor = dialoguelevel.getAuthor();
		
		// Changement de données du level
		level_tuxfisher->setNameLevel(nomlevel);
		level_tuxfisher->setNameAuthor(nomauthor);
		if (dialoguelevel.getDifficulty()==tr("Facile"))
			level_tuxfisher->setDifficulte(NIV_FACILE);
		else if (dialoguelevel.getDifficulty()==tr("Moyen"))
			level_tuxfisher->setDifficulte(NIV_MOYEN);
		else if (dialoguelevel.getDifficulty()==tr("Difficile"))
			level_tuxfisher->setDifficulte(NIV_DIFFICILE);
		else
			level_tuxfisher->setDifficulte(NIV_TOUS);
		switchWindowTitle(true);
	}
}

void MainWindow::setInfoMap()
{
	MapDialog dialoguemap(this);
	dialoguemap.loadInfo(level_tuxfisher->getNameMap(), level_tuxfisher->getSizeMap());
	dialoguemap.loadMapList(level_tuxfisher->getListNameMap());
	if (dialoguemap.exec())
	{
		QString nommap_recup;
		QSize sizemap_recup;
		nommap_recup = dialoguemap.getName();
		sizemap_recup = dialoguemap.getSize();
		if (nommap_recup != level_tuxfisher->getNameMap())
		{
			level_tuxfisher->setNameMap(nommap_recup);
			updateMapMenu();
			modification=true;
		}
		if (sizemap_recup != level_tuxfisher->getSizeMap())
		{
			editor->newMap(sizemap_recup.width(), sizemap_recup.height());
			level_tuxfisher->loadMap(editor->getMap(), sizemap_recup);
			updateMapMenu();
			update();
			modification=true;
		}
	}
}

void MainWindow::removeMap()
{
	level_tuxfisher->removeMap();
	updateMapMenu();
	loadMap();
	modification = true;
}

void MainWindow::updateMapMenu()
{
	int nummap = level_tuxfisher->getNumeroMap();
	QStringList list_map = level_tuxfisher->getListNameMap();
	menu_toolbar_map->setEnabled(false);
	menu_toolbar_map->clear();
	actions_map.clear();
	for (int i=0; i<list_map.size(); ++i)
	{
		QAction *action = new QAction(list_map.at(i), menu_toolbar_map);
		action->setIcon(createIcon(i));
		actions_map << action;
	}
	
	menu_toolbar_map->addActions(actions_map);
	menu_toolbar_map->setEnabled(true);
	action_map->setIcon(createIcon(nummap));
}

void MainWindow::loadMap()
{
	QByteArray datas;
	QSize size;
	datas = level_tuxfisher->getDatasMap();
	size = level_tuxfisher->getSizeMap();
	editor->loadMap(size.width(), size.height(), datas);
	switchWindowTitle(true);
	activateButtons();
	update();
}

void MainWindow::switchWindowTitle(bool level)
{
	if (level)
		setWindowTitle(tr("Editeur du jeu %1 - %2 de %3").arg(NAME_APPLICATION).arg(level_tuxfisher->getNameLevel()).arg(level_tuxfisher->getNameAuthor()));
	else
		setWindowTitle(tr("Editeur du jeu %1").arg(NAME_APPLICATION));
}

void MainWindow::activateButtons()
{
	action_save->setEnabled(true);
	action_upload->setEnabled(true);
	menu_level->setDisabled(false);
	menu_map->setDisabled(false);
	setCentralWidget(editor);
}

bool MainWindow::checkMap()
{
	if (level_tuxfisher->getNumeroMap()<0) // Existe-t-il une map à vérifier?
		return true;
	if (editor->isTuxPresent())
		return true;
	QMessageBox::critical (this, tr("Personnage incorrect"),
		tr("Il doit y avoir un personnage dans le level"), QMessageBox::Ok, QMessageBox::Ok);
	return false;
}

void MainWindow::createActions()
{
	action_newmap = new QAction(tr("&Nouveau jeu..."), this);
	action_newmap->setShortcut(tr("CTRL+N"));
	action_newmap->setIcon(QIcon(":/ressources/mainwindow/newjeu.png"));
	connect(action_newmap, SIGNAL(triggered()),
			this, SLOT(newLevel()));

	action_load = new QAction(tr("&Ouvrir jeu..."),this);
	action_load->setShortcut(tr("CTRL+O"));
	action_load->setIcon(QIcon(":/ressources/mainwindow/load.png"));
	connect(action_load, SIGNAL(triggered()),
			this, SLOT(load()));

	action_save = new QAction(tr("&Sauvegarder jeu..."),this);
	action_save->setShortcut(tr("CTRL+S"));
	action_save->setIcon(QIcon(":/ressources/mainwindow/save.png"));
	action_save->setEnabled(false);
	connect(action_save, SIGNAL(triggered()),
			this, SLOT(save()));

	action_upload = new QAction(tr("&Mettre en ligne..."),this);
	action_upload->setShortcut(tr("CTRL+M"));
	action_upload->setIcon(QIcon(":/ressources/mainwindow/upload.png"));
	action_upload->setEnabled(false);
	connect(action_upload, SIGNAL(triggered()),
			this, SLOT(upload()));
	
	action_quit = new QAction(tr("&Quitter"), this);
	action_quit->setIcon(QIcon(":/ressources/mainwindow/quitter.png"));
	action_quit->setShortcut(tr("CTRL+Q"));
	connect(action_quit, SIGNAL(triggered()),
			this, SLOT(close()));
	
	action_level_infolevel = new QAction(tr("Informations sur le jeu"), this);
	action_level_infolevel->setIcon(QIcon(":/ressources/mainwindow/info.png"));
	connect(action_level_infolevel, SIGNAL(triggered()),
			this, SLOT(setInfoLevel()));
			
	action_map_infomap = new QAction(tr("Informations sur le level"), this);
	action_map_infomap->setIcon(QIcon(":/ressources/mainwindow/info.png"));
	connect(action_map_infomap, SIGNAL(triggered()),
			this, SLOT(setInfoMap()));

	action_map_sortmaps = new QAction(tr("Ordonner les levelx"), this);
	action_map_sortmaps->setIcon(QIcon(":/ressources/mainwindow/sort.png"));
	connect(action_map_sortmaps, SIGNAL(triggered()),
			this, SLOT(sortMaps()));
	
	action_map_newmap = new QAction(tr("Nouveau level"), this);
	action_map_newmap->setIcon(QIcon(":/ressources/mainwindow/newlevel.png"));
	connect(action_map_newmap, SIGNAL(triggered()),
			this, SLOT(newMap()));
	
	action_map_removemap = new QAction(tr("Supprimer level"), this);
	action_map_removemap->setIcon(QIcon(":/ressources/mainwindow/remove.png"));
	connect(action_map_removemap, SIGNAL(triggered()),
			this, SLOT(removeMap()));
	
	action_about = new QAction(tr("A &Propos de l'Editeur %1").arg(NAME_APPLICATION), this);
	action_about->setShortcut(tr("CTRL+P"));
	action_about->setIcon(QIcon(":/ressources/mainwindow/logo.png"));
	connect(action_about, SIGNAL(triggered()),
			this, SLOT(about()));
	
	// Partie Map
	// Action choix map
	action_map = new QAction(tr("Level..."),this);
	menu_toolbar_map = new QMenu(tr("Reload le level"),this);
	action_map->setMenu(menu_toolbar_map);
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
	
	
	action_casemontagne = new QAction(tr("Montagne"), this);
	action_casemontagne->setCheckable(true);
	action_casemontagne->setIcon(QIcon(":/ressources/widget/montagne.png"));
	editor->selectBrush(CASE_MONTAGNE);
	action_casemontagne->setChecked(true);
	
	action_caseavalanche = new QAction(tr("Boule de neige"), this);
	action_caseavalanche->setIcon(QIcon(":/ressources/widget/avalanche.png"));
	action_caseavalanche->setCheckable(true);
			
	action_casefish = new QAction(tr("Poisson"), this);
	action_casefish->setIcon(QIcon(":/ressources/widget/fish.png"));
	action_casefish->setCheckable(true);
	
	action_caseneige = new QAction(tr("Neige"), this);
	action_caseneige->setIcon(QIcon(":/ressources/widget/neige.png"));
	action_caseneige->setCheckable(true);
			
	action_casevide = new QAction(tr("Vide"), this);
	action_casevide->setIcon(QIcon(":/ressources/widget/vide.png"));
	action_casevide->setCheckable(true);
			
	action_casetuxfisher = new QAction(NAME_APPLICATION, this);
	action_casetuxfisher->setIcon(QIcon(":/ressources/widget/tuxfisher.png"));
	action_casetuxfisher->setCheckable(true);
	
	
	groupaction = new QActionGroup(this);
	groupaction->addAction(action_casemontagne);
	groupaction->addAction(action_caseavalanche);
	groupaction->addAction(action_caseneige);
	groupaction->addAction(action_casevide);
	groupaction->addAction(action_casefish);
	groupaction->addAction(action_casetuxfisher);

	connect(groupaction, SIGNAL(triggered(QAction*)),
			this, SLOT(setBrush(QAction*)));
}

void MainWindow::createToolBar()
{
	toolbar_file = addToolBar(tr("&Fichier"));
	toolbar_file->addActions(groupaction->actions());
	toolbar_file->setMovable(false);
	
	toolbar_map = addToolBar(tr("Level"));
	toolbar_map->addAction(action_map_previous);
	toolbar_map->addAction(action_map);
	toolbar_map->addAction(action_map_next);
	toolbar_map->setMovable(false);
	toolbar_map->setVisible(false);

}

void MainWindow::setBrush(QAction *action)
{
	if (action->text()==tr("Montagne"))
		editor->selectBrush(CASE_MONTAGNE);
	if (action->text()==tr("Boule de neige"))
		editor->selectBrush(CASE_AVALANCHE);
	if (action->text()==tr("Poisson"))
		editor->selectBrush(CASE_POISSON);
	if (action->text()==NAME_APPLICATION)
		editor->selectBrush(CASE_TUXFISHER);
	if (action->text()==tr("Vide"))
		editor->selectBrush(CASE_VIDE);
	if (action->text()==tr("Neige"))
		editor->selectBrush(CASE_NEIGE);
}

QIcon MainWindow::createIcon(int i)
{
	QPixmap pixmap = pixmap_reload;
	QPainter painter(&pixmap);
	painter.setFont(QFont("Times", 20, QFont::Bold));
	painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::number(i+1));
	painter.end();
	return QIcon(pixmap);
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("A Propos de l'Editeur du jeu %1").arg(NAME_APPLICATION),
		tr("<p>Editeur du jeu %1 - Licence GPL<br/>Copyright &copy; 2008 - Xavier Monin<br/>"
		"Site web: <a href=\"http://www.tuxfisher.net\">http://www.tuxfisher.net</a></p>").arg(NAME_APPLICATION));
}

void MainWindow::createMenus()
{
	menu_file = menuBar()->addMenu(tr("&Fichier"));
	menu_file->addAction(action_newmap);
	menu_file->addAction(action_load);
	menu_file->addAction(action_save);
	menu_file->addAction(action_upload);
	menu_file->addSeparator();
	menu_file->addAction(action_quit);
	
	menu_level = menuBar()->addMenu(tr("&Jeu"));
	menu_level->addAction(action_level_infolevel);
	menu_level->addAction(action_map_sortmaps);
	menu_level->setDisabled(true);
	
	menu_map = menuBar()->addMenu(tr("N&iveau"));
	menu_map->setDisabled(true);
	menu_map->addAction(action_map_infomap);
	menu_map->addSeparator();
	menu_map->addAction(action_map_newmap);
	menu_map->addAction(action_map_removemap);
	
	menuBar()->addSeparator();
	
	menu_help = menuBar()->addMenu(tr("&Aide"));
	menu_help->addAction(action_about);
}

void MainWindow::save()
{
	if (checkMap())
	{
		level_tuxfisher->loadMap(editor->getMap());
		if (modification)
			level_tuxfisher->updateVersion();
		if (!level_tuxfisher->save())
			QMessageBox::critical (this, tr("Ecriture file"),
			tr("Impossible de save le jeu: ").arg(level_tuxfisher->getFileName()),
			QMessageBox::Ok, QMessageBox::Ok);
		else
			modification = false;
	}
}

void MainWindow::load()
{
	ChoiceLevelDialog choixleveldialog(this);
	
	if (choixleveldialog.exec())
	{
		QString filename=choixleveldialog.getFileName();
		int numeromap = choixleveldialog.getNumeroMap();
	
		if (!level_tuxfisher->loadFile(filename))
		{
			QMessageBox::information(this, tr("Error de chargement du jeu"), level_tuxfisher->getError(), QMessageBox::Ok);
			if (centralWidget()!=labelstartup)
			{
				labelstartup = new QLabel;
				labelstartup->setPixmap(QPixmap(":/ressources/mainwindow/fond.png"));
				setCentralWidget(labelstartup);
				editor = new EditorWidget(this);
			}
			toolbar_map->setVisible(false);
			menu_map->setDisabled(true);
			menu_level->setDisabled(true);
			return;
		}
		level_tuxfisher->selectMap(numeromap);
		loadMap();
		updateMapMenu();
		toolbar_map->setVisible(true);
		modification = false;
	}
}

void MainWindow::upload()
{
	int retour = QMessageBox::warning(this, tr("Mettre en ligne un jeu"),
                        tr("Etes vous sur de vouloir mettre en ligne ce jeu ?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No | QMessageBox::Escape);
	if (retour == QMessageBox::No)
		return;
	if (checkMap())
	{
		save();
		LevelUploader dialogue(this);
		dialogue.loadFileName(level_tuxfisher->getFileName());
		if (dialogue.exec())
		{
			// verif
		}
	}
}

void MainWindow::setMap(QAction *action)
{
	if (checkMap())
	{
		level_tuxfisher->loadMap(editor->getMap());
		QStringList list_map = level_tuxfisher->getListNameMap();
		for (int i=0; i<list_map.size(); ++i)
			if (list_map.at(i) == action->text())
			{
				level_tuxfisher->selectMap(i);
				loadMap();
				action_map->setIcon(createIcon(i));
			}
	}
}

void MainWindow::previousMap()
{
	if (checkMap())
	{
		level_tuxfisher->loadMap(editor->getMap());
		int num = level_tuxfisher->getNumeroMap();
		num--;
		if (level_tuxfisher->selectMap(num))
		{
			loadMap();
			action_map->setIcon(createIcon(num));
		}
	}
}

void MainWindow::nextMap()
{
	if (checkMap())
	{
		level_tuxfisher->loadMap(editor->getMap());
		int num = level_tuxfisher->getNumeroMap();
		num++;
		if (level_tuxfisher->selectMap(num))
		{
			loadMap();
			action_map->setIcon(createIcon(num));
		}
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (!modification)
		event->accept();
	else
	{
		int retour = QMessageBox::warning(this, tr("Editeur %1").arg(NAME_APPLICATION),
                        tr("Le level a été modifié.\n"
                           "Voulez vous enregistrer les modifications?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
        event->accept();
		if (retour == QMessageBox::Cancel)
			event->ignore();
		if (retour == QMessageBox::Yes)
			save();
	}
}

