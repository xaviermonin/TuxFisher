/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Auteur: Xavier Monin
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

#ifndef _MAINWINDOW_TUXFISHER_H_
#define _MAINWINDOW_TUXFISHER_H_

#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include "saviezvousdialog.h"
#include "widget_tuxfisher.h"

class QAction;
class choixNiveau_TuxFisher;
class QLabel;
class QActionGroup;
class gestionSon_TuxFisher;
class gestionNiveau_TuxFisher;
class QLCDNumber;

class MainWindow_TuxFisher : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow_TuxFisher();
	~MainWindow_TuxFisher();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void keyPressEvent(QKeyEvent *) { WTuxFisher->setFocus(); };

private:
	void chargerFichier(QString);
	void creerActions();
	void creerMenus();
	void creerBarOutils();
	void creerDocks();
	void creerBarEtat();
	void majMenuCarte();
	void changerTitreFenetre(bool);
	void lireConfiguration();
	void ecrireConfiguration();
	QIcon creerIcone(int);
	QStringList recupererGraphisme();
	QStringList recupererSon();


	Widget_TuxFisher *WTuxFisher;

	saviezVousDialog *dialogue_saviezvous;

	QMenu *menu_fichier;
	QMenu *menu_options;
	QMenu *menu_options_graphisme;
	QMenu *menu_options_son;
	QMenu *menu_aide;
	QMenu *menu_carte;
	
	QAction *action_chargerfichier;
	QAction *action_telechargerniveau;
	QAction *action_quitter;
	QAction *action_musique;
	QList<QAction *> *actions_graphisme;
	QList<QAction *> *actions_son;
	QList<QAction *> actions_carte;
	QAction *action_apropos;
	QAction *action_aide;
	QAction *action_saviezvous;
	QAction *action_carte;
	QAction *action_carte_precedente;
	QAction *action_carte_suivante;
	
	QActionGroup *groupeaction_graphisme;
	QActionGroup *groupeaction_son;
	
	QPixmap pixmap_recharger;
	
	QToolBar *baroutil_fichier;
	QToolBar *baroutil_carte;
	QToolBar *baroutil_tempsecoule;

	QTimer *timer_temps;
	QTimer *timer_gameover;
	QTimer *timer_niveautermine;

	QLCDNumber *lcd_tempsecoule;
	QLCDNumber *lcd_mouvementeffectue;
	
	QLabel *msgbaretat;
	QLabel *label_demarrage;
	
	QString aucunSon;
	QString graphismeParDefaut;
	
	gestionSon_TuxFisher *audio;
	gestionNiveau_TuxFisher *niveautuxfisher;
	
	int nb_mouvement;
	int secondes_ecoulees;

private slots:
	void niveauTermine(); // Niveau gagné
	void gameOver(); // Niveau perdu
	void changerGraphisme(QAction *);
	void changerSon(QAction *);
	void changerCarte(QAction *);
	void chargerCarte();
	void cartePrecedente();
	void carteSuivante();
	void telechargerNiveau();
	void ajouterMouvement(bool);
	void majTempsEcoule(); // Appelé par un timer. Mets à jour le compteur de temps
	void afficherMessageBarEtat(QString);
	void chargerNiveau();
	void aPropos();
	inline void aide() { QDesktopServices::openUrl(QUrl("http://www.tuxfisher.net/categorie.comment-jouer.html")); };
	inline void saviezVous() { dialogue_saviezvous->show(); };
};


#endif // !_MAINWINDOW_TUXFISHER_H_
