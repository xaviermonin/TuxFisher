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

#include <QtGui>
#include <iostream>
#include "mainwindow_editeur.h"
#include "widget_editeur.h"
#include "../Commun/gestionniveau_tuxfisher.h"
#include "niveaudialog_editeur.h"
#include "cartedialog_editeur.h"
#include "mettreenlignedialog_editeur.h"
#include "../Commun/choixniveaudialog_tuxfisher.h"
#include "ordrecartedialog.h"
#include "../Commun/definitions.h"

MainWindow_Editeur::MainWindow_Editeur()
{
	nomfichier="";
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	setWindowIcon(QIcon(":/ressources/mainwindow/logo.png"));
	modification=false;
	editeur = new Widget_Editeur(this);
	niveautuxfisher = new gestionNiveau_TuxFisher;
	pixmap_recharger = QPixmap(":/ressources/mainwindow/recharger.png");
	creerActions();
	creerMenus();
	creerBarOutils();
	changerTitreFenetre(false);
	labeldemarrage = new QLabel;
	labeldemarrage->setPixmap(QPixmap(":/ressources/mainwindow/fond.png"));
	setCentralWidget(labeldemarrage);
	connect(editeur, SIGNAL(modifications()), this, SLOT(modifications()));
	connect(menu_baroutil_carte, SIGNAL(triggered(QAction *)), this, SLOT(changerCarte(QAction *)));
}

MainWindow_Editeur::~MainWindow_Editeur()
{
	delete niveautuxfisher;
}

void MainWindow_Editeur::nouveauNiveau()
{
	nomfichier="";
	niveauDialog_Editeur dialog(this);
	if (dialog.exec())
	{
		QString nomniveau;
		QString nomauteur;
		
		// Chargement des données
		nomniveau = dialog.recupererNom();
		nomauteur = dialog.recupererAuteur();
		
		// Création du niveau
		niveautuxfisher->nouveauNiveau(nomniveau, nomauteur);
		if (dialog.recupererDifficulte()==tr("Facile"))
			niveautuxfisher->changerDifficulte(NIV_FACILE);
		else if (dialog.recupererDifficulte()==tr("Moyen"))
			niveautuxfisher->changerDifficulte(NIV_MOYEN);
		else if (dialog.recupererDifficulte()==tr("Difficile"))
			niveautuxfisher->changerDifficulte(NIV_DIFFICILE);
		else
			niveautuxfisher->changerDifficulte(NIV_TOUS);
		
		// Création d'une carte
		nouvelleCarte();
		changerTitreFenetre(true);
	}
}

void MainWindow_Editeur::nouvelleCarte()
{
	if (verifierCarte())
	{
		carteDialog_Editeur dialoguecarte(this);
		if (niveautuxfisher->recupererNumeroCarte()>=0)
		{
			niveautuxfisher->chargerCarte(editeur->recupererCarte());
			dialoguecarte.chargerListeCarte(niveautuxfisher->recupererListeNomCarte());
		}
		if (dialoguecarte.exec())
		{
			QString nomcarte;
			QByteArray carte;
			taillecarte = dialoguecarte.recupererTaille();
			nomcarte = dialoguecarte.recupererNom();

			editeur->nouvelleCarte(taillecarte.width(), taillecarte.height());
			carte = editeur->recupererCarte();
			niveautuxfisher->nouvelleCarte(nomcarte, taillecarte, carte);
			activerFonctions();
			majMenuCarte();
			baroutil_carte->setVisible(true);
			update();
			modification=true;
		}
	}
}

void MainWindow_Editeur::ordonnerCartes()
{
	niveautuxfisher->chargerCarte(editeur->recupererCarte());
	ordreCarteDialog ordonnercartes(this);
	ordonnercartes.chargerStringList(niveautuxfisher->recupererListeNomCarte());
	if (ordonnercartes.exec())
	{
		QMap <int, int> classement = ordonnercartes.recupererClassement();

		QMap <int, int> classement_en_cours; // On crée un classement qui servira de controle
		for (int count=0; count<classement.count(); count++)
			classement_en_cours[count]=count;
		
		for (int count=0; count<classement.count(); count++)
		{
			if (classement[count]!=classement_en_cours[count]) // On ne remplace pas si c'est déja bon
			{
				int i; // Stock les coordonnées de la cible (pour le remplacement)
				modification=true;
				for(i=0; i<classement_en_cours.count(); i++)
					if (classement_en_cours[i]==classement[count]) // On sait maintenant a quel endroit est la cible
						break;

				niveautuxfisher->echangerCartes(count,i); // On échange donc les deux, la coordonnée count sera la bonne carte!

				int temp=classement_en_cours[count]; // On met a jour le classement puisqu'il vient de changer,
				classement_en_cours[count]=classement_en_cours[i]; // les coordonnées des cibles ont changés puisqu'on a échanger des cibles
				classement_en_cours[i]=temp;
			}
		}
		majMenuCarte();
		chargerCarte();
	}
}

void MainWindow_Editeur::changerInfoNiveau()
{
	niveauDialog_Editeur dialogueniveau(this);
	dialogueniveau.chargerNiveauDialog(niveautuxfisher->recupererNomNiveau(), niveautuxfisher->recupererNomAuteur(), niveautuxfisher->recupererDifficulte());
	if (dialogueniveau.exec())
	{
		// Chargement des données
		QString nomniveau = dialogueniveau.recupererNom();
		QString nomauteur = dialogueniveau.recupererAuteur();
		
		// Changement de données du niveau
		niveautuxfisher->changerNomNiveau(nomniveau);
		niveautuxfisher->changerNomAuteur(nomauteur);
		if (dialogueniveau.recupererDifficulte()==tr("Facile"))
			niveautuxfisher->changerDifficulte(NIV_FACILE);
		else if (dialogueniveau.recupererDifficulte()==tr("Moyen"))
			niveautuxfisher->changerDifficulte(NIV_MOYEN);
		else if (dialogueniveau.recupererDifficulte()==tr("Difficile"))
			niveautuxfisher->changerDifficulte(NIV_DIFFICILE);
		else
			niveautuxfisher->changerDifficulte(NIV_TOUS);
		changerTitreFenetre(true);
	}
}

void MainWindow_Editeur::changerInfoCarte()
{
	carteDialog_Editeur dialoguecarte(this);
	dialoguecarte.chargerInfo(niveautuxfisher->recupererNomCarte(), niveautuxfisher->recupererTailleCarte());
	dialoguecarte.chargerListeCarte(niveautuxfisher->recupererListeNomCarte());
	if (dialoguecarte.exec())
	{
		QString nomcarte_recup;
		QSize taillecarte_recup;
		nomcarte_recup = dialoguecarte.recupererNom();
		taillecarte_recup = dialoguecarte.recupererTaille();
		if (nomcarte_recup != niveautuxfisher->recupererNomCarte())
		{
			niveautuxfisher->changerNomCarte(nomcarte_recup);
			majMenuCarte();
			modification=true;
		}
		if (taillecarte_recup != niveautuxfisher->recupererTailleCarte())
		{
			editeur->nouvelleCarte(taillecarte_recup.width(), taillecarte_recup.height());
			niveautuxfisher->chargerCarte(editeur->recupererCarte(), taillecarte_recup);
			majMenuCarte();
			update();
			modification=true;
		}
	}
}

void MainWindow_Editeur::supprimerCarte()
{
	niveautuxfisher->supprimerCarte();
	majMenuCarte();
	chargerCarte();
	modification = true;
}

void MainWindow_Editeur::majMenuCarte()
{
	int numcarte = niveautuxfisher->recupererNumeroCarte();
	QStringList liste_carte = niveautuxfisher->recupererListeNomCarte();
	menu_baroutil_carte->setEnabled(false);
	menu_baroutil_carte->clear();
	actions_carte.clear();
	for (int i=0; i<liste_carte.size(); i++)
	{
		QAction *action = new QAction(liste_carte.at(i), menu_baroutil_carte);
		action->setIcon(creerIcone(i));
		actions_carte << action;
	}
	
	menu_baroutil_carte->addActions(actions_carte);
	menu_baroutil_carte->setEnabled(true);
	action_carte->setIcon(creerIcone(numcarte));
}

void MainWindow_Editeur::chargerCarte()
{
	QByteArray donnes;
	QSize taille;
	donnes = niveautuxfisher->recupererDonnesCarte();
	taille = niveautuxfisher->recupererTailleCarte();
	editeur->chargerCarte(taille.width(), taille.height(), donnes);
	changerTitreFenetre(true);
	activerFonctions();
	update();
}

void MainWindow_Editeur::changerTitreFenetre(bool niveau)
{
	if (niveau)
		setWindowTitle(tr("Editeur du jeu %1 - %2 de %3").arg(NOM_APPLICATION).arg(niveautuxfisher->recupererNomNiveau()).arg(niveautuxfisher->recupererNomAuteur()));
	else
		setWindowTitle(tr("Editeur du jeu %1").arg(NOM_APPLICATION));
}

void MainWindow_Editeur::activerFonctions()
{
	action_sauvegarder->setEnabled(true);
	action_mettreenligne->setEnabled(true);
	menu_niveau->setDisabled(false);
	menu_carte->setDisabled(false);
	setCentralWidget(editeur);
}

bool MainWindow_Editeur::verifierCarte()
{
	if (niveautuxfisher->recupererNumeroCarte()<0) // Existe-t-il une carte à vérifier?
		return true;
	if (editeur->tuxfisherPresent())
		return true;
	QMessageBox::critical (this, tr("Personnage incorrect"),
		tr("Il doit y avoir un personnage dans le niveau"), QMessageBox::Ok, QMessageBox::Ok);
	return false;
}

void MainWindow_Editeur::creerActions()
{
	action_nouvellecarte = new QAction(tr("&Nouveau jeu..."), this);
	action_nouvellecarte->setShortcut(tr("CTRL+N"));
	action_nouvellecarte->setIcon(QIcon(":/ressources/mainwindow/nouveaujeu.png"));
	connect(action_nouvellecarte, SIGNAL(triggered()),
			this, SLOT(nouveauNiveau()));

	action_charger = new QAction(tr("&Ouvrir jeu..."),this);
	action_charger->setShortcut(tr("CTRL+O"));
	action_charger->setIcon(QIcon(":/ressources/mainwindow/charger.png"));
	connect(action_charger, SIGNAL(triggered()),
			this, SLOT(charger()));

	action_sauvegarder = new QAction(tr("&Sauvegarder jeu..."),this);
	action_sauvegarder->setShortcut(tr("CTRL+S"));
	action_sauvegarder->setIcon(QIcon(":/ressources/mainwindow/sauvegarder.png"));
	action_sauvegarder->setEnabled(false);
	connect(action_sauvegarder, SIGNAL(triggered()),
			this, SLOT(sauvegarder()));

	action_mettreenligne = new QAction(tr("&Mettre en ligne..."),this);
	action_mettreenligne->setShortcut(tr("CTRL+M"));
	action_mettreenligne->setIcon(QIcon(":/ressources/mainwindow/mettreenligne.png"));
	action_mettreenligne->setEnabled(false);
	connect(action_mettreenligne, SIGNAL(triggered()),
			this, SLOT(mettreenligne()));
	
	action_quitter = new QAction(tr("&Quitter"), this);
	action_quitter->setIcon(QIcon(":/ressources/mainwindow/quitter.png"));
	action_quitter->setShortcut(tr("CTRL+Q"));
	connect(action_quitter, SIGNAL(triggered()),
			this, SLOT(close()));
	
	action_niveau_infoniveau = new QAction(tr("Informations sur le jeu"), this);
	action_niveau_infoniveau->setIcon(QIcon(":/ressources/mainwindow/info.png"));
	connect(action_niveau_infoniveau, SIGNAL(triggered()),
			this, SLOT(changerInfoNiveau()));
			
	action_carte_infocarte = new QAction(tr("Informations sur le niveau"), this);
	action_carte_infocarte->setIcon(QIcon(":/ressources/mainwindow/info.png"));
	connect(action_carte_infocarte, SIGNAL(triggered()),
			this, SLOT(changerInfoCarte()));

	action_carte_ordonnercartes = new QAction(tr("Ordonner les niveaux"), this);
	action_carte_ordonnercartes->setIcon(QIcon(":/ressources/mainwindow/ordonner.png"));
	connect(action_carte_ordonnercartes, SIGNAL(triggered()),
			this, SLOT(ordonnerCartes()));
	
	action_carte_nouvellecarte = new QAction(tr("Nouveau niveau"), this);
	action_carte_nouvellecarte->setIcon(QIcon(":/ressources/mainwindow/nouveauniveau.png"));
	connect(action_carte_nouvellecarte, SIGNAL(triggered()),
			this, SLOT(nouvelleCarte()));
	
	action_carte_supprimercarte = new QAction(tr("Supprimer niveau"), this);
	action_carte_supprimercarte->setIcon(QIcon(":/ressources/mainwindow/supprimer.png"));
	connect(action_carte_supprimercarte, SIGNAL(triggered()),
			this, SLOT(supprimerCarte()));
	
	action_apropos = new QAction(tr("A &Propos de l'Editeur %1").arg(NOM_APPLICATION), this);
	action_apropos->setShortcut(tr("CTRL+P"));
	action_apropos->setIcon(QIcon(":/ressources/mainwindow/logo.png"));
	connect(action_apropos, SIGNAL(triggered()),
			this, SLOT(aPropos()));
	
	// Partie Carte
	// Action choix carte
	action_carte = new QAction(tr("Niveau..."),this);
	menu_baroutil_carte = new QMenu(tr("Recharger le niveau"),this);
	action_carte->setMenu(menu_baroutil_carte);
	connect(action_carte, SIGNAL(triggered()),
			this, SLOT(chargerCarte()));

	// Actions carte suivante et précedente
	action_carte_precedente = new QAction(tr("Niveau précédent"),this);
	action_carte_precedente->setIcon(QIcon(":/ressources/mainwindow/precedent.png"));
	connect(action_carte_precedente, SIGNAL(triggered()),
			this, SLOT(cartePrecedente()));
	
	action_carte_suivante = new QAction(tr("Niveau suivant"),this);
	action_carte_suivante->setIcon(QIcon(":/ressources/mainwindow/suivant.png"));
	connect(action_carte_suivante, SIGNAL(triggered()),
			this, SLOT(carteSuivante()));
	
	
	action_casemontagne = new QAction(tr("Montagne"), this);
	action_casemontagne->setCheckable(true);
	action_casemontagne->setIcon(QIcon(":/ressources/widget/montagne.png"));
	editeur->selectionnerPinceau(CASE_MONTAGNE);
	action_casemontagne->setChecked(true);
	
	action_caseavalanche = new QAction(tr("Boule de neige"), this);
	action_caseavalanche->setIcon(QIcon(":/ressources/widget/avalanche.png"));
	action_caseavalanche->setCheckable(true);
			
	action_casepoisson = new QAction(tr("Poisson"), this);
	action_casepoisson->setIcon(QIcon(":/ressources/widget/poisson.png"));
	action_casepoisson->setCheckable(true);
	
	action_caseneige = new QAction(tr("Neige"), this);
	action_caseneige->setIcon(QIcon(":/ressources/widget/neige.png"));
	action_caseneige->setCheckable(true);
			
	action_casevide = new QAction(tr("Vide"), this);
	action_casevide->setIcon(QIcon(":/ressources/widget/vide.png"));
	action_casevide->setCheckable(true);
			
	action_casetuxfisher = new QAction(NOM_APPLICATION, this);
	action_casetuxfisher->setIcon(QIcon(":/ressources/widget/tuxfisher.png"));
	action_casetuxfisher->setCheckable(true);
	
	
	groupaction = new QActionGroup(this);
	groupaction->addAction(action_casemontagne);
	groupaction->addAction(action_caseavalanche);
	groupaction->addAction(action_caseneige);
	groupaction->addAction(action_casevide);
	groupaction->addAction(action_casepoisson);
	groupaction->addAction(action_casetuxfisher);

	connect(groupaction, SIGNAL(triggered(QAction*)),
			this, SLOT(changerPinceau(QAction*)));
}

void MainWindow_Editeur::creerBarOutils()
{
	baroutil_fichier = addToolBar(tr("&Fichier"));
	baroutil_fichier->addActions(groupaction->actions());
	baroutil_fichier->setMovable(false);
	
	baroutil_carte = addToolBar(tr("Niveau"));
	baroutil_carte->addAction(action_carte_precedente);
	baroutil_carte->addAction(action_carte);
	baroutil_carte->addAction(action_carte_suivante);
	baroutil_carte->setMovable(false);
	baroutil_carte->setVisible(false);

}

void MainWindow_Editeur::changerPinceau(QAction *action)
{
	if (action->text()==tr("Montagne"))
		editeur->selectionnerPinceau(CASE_MONTAGNE);
	if (action->text()==tr("Boule de neige"))
		editeur->selectionnerPinceau(CASE_AVALANCHE);
	if (action->text()==tr("Poisson"))
		editeur->selectionnerPinceau(CASE_POISSON);
	if (action->text()==NOM_APPLICATION)
		editeur->selectionnerPinceau(CASE_TUXFISHER);
	if (action->text()==tr("Vide"))
		editeur->selectionnerPinceau(CASE_VIDE);
	if (action->text()==tr("Neige"))
		editeur->selectionnerPinceau(CASE_NEIGE);
}

QIcon MainWindow_Editeur::creerIcone(int i)
{
	QPixmap pixmap = pixmap_recharger;
	QPainter painter(&pixmap);
	painter.setFont(QFont("Times", 20, QFont::Bold));
	painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::number(i+1));
	painter.end();
	return QIcon(pixmap);
}

void MainWindow_Editeur::aPropos()
{
	QMessageBox::about(this, tr("A Propos de l'Editeur du jeu %1").arg(NOM_APPLICATION),
		tr("<p>Editeur du jeu %1 - Licence GPL<br/>Copyright &copy; 2008 - Xavier Monin<br/>"
		"Site web: <a href=\"http://www.tuxfisher.net\">http://www.tuxfisher.net</a></p>").arg(NOM_APPLICATION));
}

void MainWindow_Editeur::creerMenus()
{
	menu_fichier = menuBar()->addMenu(tr("&Fichier"));
	menu_fichier->addAction(action_nouvellecarte);
	menu_fichier->addAction(action_charger);
	menu_fichier->addAction(action_sauvegarder);
	menu_fichier->addAction(action_mettreenligne);
	menu_fichier->addSeparator();
	menu_fichier->addAction(action_quitter);
	
	menu_niveau = menuBar()->addMenu(tr("&Jeu"));
	menu_niveau->addAction(action_niveau_infoniveau);
	menu_niveau->addAction(action_carte_ordonnercartes);
	menu_niveau->setDisabled(true);
	
	menu_carte = menuBar()->addMenu(tr("N&iveau"));
	menu_carte->setDisabled(true);
	menu_carte->addAction(action_carte_infocarte);
	menu_carte->addSeparator();
	menu_carte->addAction(action_carte_nouvellecarte);
	menu_carte->addAction(action_carte_supprimercarte);
	
	menuBar()->addSeparator();
	
	menu_aide = menuBar()->addMenu(tr("&Aide"));
	menu_aide->addAction(action_apropos);
}

void MainWindow_Editeur::sauvegarder()
{
	if (verifierCarte())
	{
		niveautuxfisher->chargerCarte(editeur->recupererCarte());
		if (modification)
			niveautuxfisher->majVersion();
		if (!niveautuxfisher->sauvegarder())
			QMessageBox::critical (this, tr("Ecriture fichier"),
			tr("Impossible de sauvegarder le jeu: ").arg(niveautuxfisher->recupererNomFichier()),
			QMessageBox::Ok, QMessageBox::Ok);
		else
			modification = false;
	}
}

void MainWindow_Editeur::charger()
{
	choixNiveauDialog_TuxFisher choixniveaudialog(this);
	
	if (choixniveaudialog.exec())
	{
		QString nomfichier=choixniveaudialog.recupererNomFichier();
		int numerocarte = choixniveaudialog.recupererNumeroCarte();
	
		if (!niveautuxfisher->chargerFichier(nomfichier))
		{
			QMessageBox::information(this, tr("Erreur de chargement du jeu"), niveautuxfisher->recupererErreur(), QMessageBox::Ok);
			if (centralWidget()!=labeldemarrage)
			{
				labeldemarrage = new QLabel;
				labeldemarrage->setPixmap(QPixmap(":/ressources/mainwindow/fond.png"));
				setCentralWidget(labeldemarrage);
				editeur = new Widget_Editeur(this);
			}
			baroutil_carte->setVisible(false);
			menu_carte->setDisabled(true);
			menu_niveau->setDisabled(true);
			return;
		}
		niveautuxfisher->selectionnerCarte(numerocarte);
		chargerCarte();
		majMenuCarte();
		baroutil_carte->setVisible(true);
		modification = false;
	}
}

void MainWindow_Editeur::mettreenligne()
{
	int retour = QMessageBox::warning(this, tr("Mettre en ligne un jeu"),
                        tr("Etes vous sur de vouloir mettre en ligne ce jeu ?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No | QMessageBox::Escape);
	if (retour == QMessageBox::No)
		return;
	if (verifierCarte())
	{
		sauvegarder();
		mettreEnLigneDialog_Editeur dialogue(this);
		dialogue.chargerNomFichier(niveautuxfisher->recupererNomFichier());
		if (dialogue.exec())
		{
			// verif
		}
	}
}

void MainWindow_Editeur::changerCarte(QAction *action)
{
	if (verifierCarte())
	{
		niveautuxfisher->chargerCarte(editeur->recupererCarte());
		QStringList liste_carte = niveautuxfisher->recupererListeNomCarte();
		for (int i=0; i<liste_carte.size(); i++)
			if (liste_carte.at(i) == action->text())
			{
				niveautuxfisher->selectionnerCarte(i);
				chargerCarte();
				action_carte->setIcon(creerIcone(i));
			}
	}
}

void MainWindow_Editeur::cartePrecedente()
{
	if (verifierCarte())
	{
		niveautuxfisher->chargerCarte(editeur->recupererCarte());
		int num = niveautuxfisher->recupererNumeroCarte();
		num--;
		if (niveautuxfisher->selectionnerCarte(num))
		{
			chargerCarte();
			action_carte->setIcon(creerIcone(num));
		}
	}
}

void MainWindow_Editeur::carteSuivante()
{
	if (verifierCarte())
	{
		niveautuxfisher->chargerCarte(editeur->recupererCarte());
		int num = niveautuxfisher->recupererNumeroCarte();
		num++;
		if (niveautuxfisher->selectionnerCarte(num))
		{
			chargerCarte();
			action_carte->setIcon(creerIcone(num));
		}
	}
}

void MainWindow_Editeur::closeEvent(QCloseEvent *event)
{
	if (!modification)
		event->accept();
	else
	{
		int retour = QMessageBox::warning(this, tr("Editeur %1").arg(NOM_APPLICATION),
                        tr("Le niveau a été modifié.\n"
                           "Voulez vous enregistrer les modifications?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
        event->accept();
		if (retour == QMessageBox::Cancel)
			event->ignore();
		if (retour == QMessageBox::Yes)
			sauvegarder();
	}
}

