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

#ifndef _MAINWINDOW_EDITEUR_H_
#define _MAINWINDOW_EDITEUR_H_

#include <QMainWindow>
#include <QActionGroup>
#include <QPixmap>
#include <QLabel>
#include <QIcon>

class Widget_Editeur;
class gestionNiveau_TuxFisher;

class MainWindow_Editeur : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow_Editeur();
	~MainWindow_Editeur();
protected:
	void closeEvent(QCloseEvent *event);
private:
	void creerMenus();
	void creerActions();
	void creerBarOutils();
	QIcon creerIcone(int);
	void changerTitreFenetre(bool);
	void activerFonctions();
	void majMenuCarte();
	bool verifierCarte();
	
	Widget_Editeur *editeur;
	gestionNiveau_TuxFisher *niveautuxfisher;
	
	QMenu *menu_fichier;
	QMenu *menu_niveau;
	QMenu *menu_carte;
	QMenu *menu_aide;

	QMenu *menu_baroutil_carte;
	
	QAction *action_nouvellecarte;
	QAction *action_quitter;
	QAction *action_apropos;
	QAction *action_sauvegarder;
	QAction *action_mettreenligne;
	QAction *action_charger;
	
	QList<QAction *> actions_carte;
	QAction *action_carte;
	QAction *action_carte_precedente;
	QAction *action_carte_suivante;

	QAction *action_niveau_infoniveau;
	
	QAction *action_carte_infocarte;
	QAction *action_carte_ordonnercartes;
	QAction *action_carte_nouvellecarte;
	QAction *action_carte_supprimercarte;
	
	QAction *action_casemontagne;
	QAction *action_caseavalanche;
	QAction *action_casepoisson;
	QAction *action_casetuxfisher;
	QAction *action_casevide;
	QAction *action_caseneige;
	
	QActionGroup *groupaction;
	
	QPixmap pixmap_recharger;
	
	QToolBar *baroutil_fichier;
	QToolBar *baroutil_carte;
	
	QLabel *labeldemarrage;
	
	QString nomfichier;
	
	QSize taillecarte;
	
	bool modification;

private slots:
	void nouveauNiveau();
	void nouvelleCarte();
	void changerInfoNiveau();
	void changerInfoCarte();
	void supprimerCarte();
	void aPropos();
	void changerPinceau(QAction *);
	void sauvegarder();
	void mettreenligne();
	void charger();
	void modifications() { modification=true; }
	void carteSuivante();
	void cartePrecedente();
	void chargerCarte();
	void changerCarte(QAction *);
	void ordonnerCartes();
};

#endif // !_MAINWINDOW_EDITEUR_H_
