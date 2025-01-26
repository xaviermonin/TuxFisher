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

#include <iostream>
#include <QtGui>
#include "gestionson_tuxfisher.h"
#include "mainwindow_tuxfisher.h"
#include "widget_tuxfisher.h"
#include "../Commun/choixniveaudialog_tuxfisher.h"
#include "telechargerniveaudialog.h"
#include "../Commun/gestionniveau_tuxfisher.h"
#include "saviezvousdialog.h"
#include "../Commun/definitions.h"

MainWindow_TuxFisher::MainWindow_TuxFisher()
{
	QCoreApplication::setOrganizationName("Xavier Monin");
	QCoreApplication::setOrganizationDomain("tuxfisher.net");
	QCoreApplication::setApplicationName(NOM_APPLICATION);

	QDir::setCurrent(QCoreApplication::applicationDirPath());
	setWindowIcon(QIcon(":/ressources/mainwindow/logo.png"));

	audio = gestionSon_TuxFisher::getInstance();
	WTuxFisher = new Widget_TuxFisher(this);
	niveautuxfisher = new gestionNiveau_TuxFisher;
	
	setCentralWidget(WTuxFisher);

	changerTitreFenetre(false);
	
	aucunSon = tr("Aucun son");
	graphismeParDefaut = tr("Classique");

	creerActions();
	creerMenus();
	creerBarOutils();
	creerDocks();
	creerBarEtat();

	dialogue_saviezvous = new saviezVousDialog(this);
	
	// Créations et configurations des timers
	timer_temps = new QTimer(this);
	timer_gameover = new QTimer(this);
	timer_gameover->setInterval(2000);
	timer_gameover->setSingleShot(true);
	timer_niveautermine = new QTimer(this);
	timer_niveautermine->setInterval(2000);
	timer_niveautermine->setSingleShot(true);

	pixmap_recharger = QPixmap(":/ressources/mainwindow/recharger.png");
	nb_mouvement=0;
	secondes_ecoulees=0;
	
	connect(WTuxFisher, SIGNAL(gameOver()), this, SLOT(gameOver()));
	connect(WTuxFisher, SIGNAL(niveauTermine()), this, SLOT(niveauTermine()));
	connect(WTuxFisher, SIGNAL(ajouterMouvement(bool)), this, SLOT(ajouterMouvement(bool)));

	connect(timer_temps, SIGNAL(timeout()), this, SLOT(majTempsEcoule()));
	connect(timer_gameover, SIGNAL(timeout()), this, SLOT(chargerCarte()));
	connect(timer_niveautermine, SIGNAL(timeout()), this, SLOT(carteSuivante()));

	connect(menu_carte, SIGNAL(triggered(QAction *)), this, SLOT(changerCarte(QAction *)));
	
	lireConfiguration();
	afficherMessageBarEtat(tr("%1 est chargé").arg(NOM_APPLICATION));
}

MainWindow_TuxFisher::~MainWindow_TuxFisher()
{
	gestionSon_TuxFisher::kill();
	delete niveautuxfisher;
	delete actions_graphisme;
	delete actions_son;
}

void MainWindow_TuxFisher::majTempsEcoule()
{
	if (!isActiveWindow())
		return;
	secondes_ecoulees+=1;
	QTime time;
	time= time.addSecs(secondes_ecoulees);
	QString temps = time.toString("mm:ss");
	lcd_tempsecoule->display(temps);
}

QIcon MainWindow_TuxFisher::creerIcone(int i)
{
	QPixmap pixmap = pixmap_recharger;
	QPainter painter(&pixmap);
	painter.setFont(QFont("Times", 18, QFont::Bold));
	painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::number(i+1));
	painter.end();
	return QIcon(pixmap);
}

void MainWindow_TuxFisher::creerActions()
{
	action_chargerfichier = new QAction(tr("&Charger Partie..."), this);
	action_chargerfichier->setShortcut(tr("CTRL+C"));
	action_chargerfichier->setIcon(QIcon(":/ressources/mainwindow/charger.png"));
	connect(action_chargerfichier, SIGNAL(triggered()),
			this, SLOT(chargerNiveau()));
	
	action_telechargerniveau = new QAction(tr("&Télécharger partie..."), this);
	action_telechargerniveau->setShortcut(tr("CTRL+T"));
	action_telechargerniveau->setIcon(QIcon(":/ressources/mainwindow/telecharger.png"));
	connect(action_telechargerniveau, SIGNAL(triggered()),
			this, SLOT(telechargerNiveau()));
	
	action_quitter = new QAction(tr("&Quitter"), this);
	action_quitter->setShortcut(tr("CTRL+Q"));
	action_quitter->setIcon(QIcon(":/ressources/mainwindow/quitter.png"));
	connect(action_quitter, SIGNAL(triggered()),
			this, SLOT(close()));
	
	action_aide = new QAction(tr("Aide"), this);
	action_aide->setShortcut(tr("F1"));
	action_aide->setIcon(QIcon(":/ressources/mainwindow/aide.png"));
	connect(action_aide, SIGNAL(triggered()),
			this, SLOT(aide()));

	action_saviezvous = new QAction(tr("&Le saviez vous?"), this);
	action_saviezvous->setShortcut(tr("CTRL+L"));
	action_saviezvous->setIcon(QIcon(":/ressources/mainwindow/saviezvous.png"));
	connect(action_saviezvous, SIGNAL(triggered()),
			this, SLOT(saviezVous()));
	
	action_apropos = new QAction(tr("A &Propos de %1").arg(NOM_APPLICATION), this);
	action_apropos->setShortcut(tr("CTRL+P"));
	action_apropos->setIcon(QIcon(":/ressources/mainwindow/logo.png"));
	connect(action_apropos, SIGNAL(triggered()),
			this, SLOT(aPropos()));
	
	// Partie Carte
	// Action choix carte
	action_carte = new QAction(tr("Niveau..."),this);
	menu_carte = new QMenu(tr("Niveau"),this);
	action_carte->setMenu(menu_carte);
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

	// Partie Options
	// Actions graphisme
	actions_graphisme = new QList<QAction *>;
	QStringList listegraphisme = recupererGraphisme();
	for (int i=0; i<listegraphisme.size(); i++)
	{
		actions_graphisme->append(new QAction(listegraphisme.at(i),this));
		actions_graphisme->at(i)->setCheckable(true);
	}
	
	groupeaction_graphisme = new QActionGroup(this);
	for (int i=0; i<actions_graphisme->size(); i++)
		groupeaction_graphisme->addAction(actions_graphisme->at(i));
	
	// Action musique
	action_musique = new QAction(tr("Musique"), this);
	action_musique->setCheckable(true);
	connect(action_musique, SIGNAL(toggled(bool)),
			audio, SLOT(jouerMusique(bool)));
	
	// Actions son
	actions_son = new QList<QAction *>;
	QStringList listeson = recupererSon();
	for (int i=0; i<listeson.size(); i++)
	{
		actions_son->append(new QAction(listeson.at(i),this));
		actions_son->at(i)->setCheckable(true);
	}
	
	groupeaction_son = new QActionGroup(this);
	for (int i=0; i<actions_son->size(); i++)
		groupeaction_son->addAction(actions_son->at(i));

	connect(groupeaction_graphisme, SIGNAL(triggered(QAction*)),
			this, SLOT(changerGraphisme(QAction*)));
	connect(groupeaction_son, SIGNAL(triggered(QAction*)),
			this, SLOT(changerSon(QAction*)));
}

void MainWindow_TuxFisher::chargerCarte()
{
	timer_gameover->stop();
	if (!niveautuxfisher->selectionnerCarte(niveautuxfisher->recupererNumeroCarte()))
		return;
	QByteArray donnes;
	QSize taille;
	donnes = niveautuxfisher->recupererDonnesCarte();
	taille = niveautuxfisher->recupererTailleCarte();
	timer_temps->stop();
	lcd_tempsecoule->display("00:00");
	lcd_mouvementeffectue->display("0");
	secondes_ecoulees=0;
	nb_mouvement=0;
	WTuxFisher->chargerMap(taille.width(), taille.height(), donnes);
	majMenuCarte();
	afficherMessageBarEtat(tr("Chargement de la partie terminée."));
}

void MainWindow_TuxFisher::changerTitreFenetre(bool niveau)
{
	if (niveau)
		setWindowTitle(tr("%1 - %2 de %3").arg(NOM_APPLICATION).arg(niveautuxfisher->recupererNomNiveau()).arg(niveautuxfisher->recupererNomAuteur()));
	else
		setWindowTitle(NOM_APPLICATION);
}

void MainWindow_TuxFisher::lireConfiguration()
{
	QSettings configuration("Monin Xavier", NOM_APPLICATION);
	bool premierlancement = configuration.value("premierlancement", true).toBool();
	bool saviezvous = configuration.value("saviezvous", true).toBool();
	bool musique = configuration.value("musique", false).toBool();
	QString stringSon = configuration.value("son").toString();
	QString stringGraphisme = configuration.value("graphisme", graphismeParDefaut).toString();
	
	bool ok=false;

	for (int count=0; count<actions_son->count(); count++)
		if (actions_son->at(count)->text()==stringSon)
		{
			actions_son->at(count)->trigger(); // Si on trouve le son enregistré on le sélectionne
			ok=true;
			break;
		}

	if (!ok && actions_son->size()>0) // Si on a rien trouvé
		if (actions_son->at(0)->text()==aucunSon)
		{
			if (actions_son->size()>1)
				actions_son->at(1)->trigger(); // On sélectionne le premier vrai son
			else
				actions_son->at(0)->trigger(); // On sélectionne le choix aucunson si il est le seul disponible
		}
		else
			actions_son->at(0)->trigger(); // On selectionne le premier vrai son

	ok=false;

	for (int count=0; count<actions_graphisme->count(); count++)
		if (actions_graphisme->at(count)->text()==stringGraphisme)
		{
			actions_graphisme->at(count)->trigger();
			ok=true;
			break;
		}
	if (!ok)
		actions_graphisme->at(0)->trigger();

	action_musique->setChecked(musique);
	if (saviezvous)
	{
		if (premierlancement)
			dialogue_saviezvous->afficherPresentation();

		dialogue_saviezvous->ChargerEtatCheckBox(false);
		dialogue_saviezvous->show();
	}
	else
		dialogue_saviezvous->ChargerEtatCheckBox(true);
}

void MainWindow_TuxFisher::ecrireConfiguration()
{
	QSettings configuration("Monin Xavier", QString("%1").arg(NOM_APPLICATION));
	configuration.setValue("premierlancement" , false);
	configuration.setValue("musique" , action_musique->isChecked());
	configuration.setValue("saviezvous" , dialogue_saviezvous->afficher());
	configuration.setValue("son" , groupeaction_son->checkedAction()->text());
	configuration.setValue("graphisme" , groupeaction_graphisme->checkedAction()->text());
}

void MainWindow_TuxFisher::ajouterMouvement(bool ajouter)
{
	if (!timer_temps->isActive() && ajouter)
		timer_temps->start(1000);
	if (ajouter)
		nb_mouvement++;
	if (!ajouter && nb_mouvement>=0)
		nb_mouvement--;
	lcd_mouvementeffectue->display(QString::number(nb_mouvement));
}

void MainWindow_TuxFisher::majMenuCarte()
{
	menu_carte->setEnabled(false);
	menu_carte->clear();
	actions_carte.clear();

	QStringList liste_carte=niveautuxfisher->recupererListeNomCarte();
	int cartesdebloquees=niveautuxfisher->recupererCartesDebloquees();
	int nbcartes=liste_carte.count();

	for (int i=0; i<nbcartes; i++)
	{
		if (i>=cartesdebloquees)
				liste_carte[i]=tr("Niveau non débloqué");
		QAction *action = new QAction(liste_carte.at(i), menu_carte);
		if (i>=cartesdebloquees)
			action->setDisabled(true);
		action->setIcon(creerIcone(i));
		actions_carte << action;
	}

	int carteselectionnee=niveautuxfisher->recupererNumeroCarte();

	if (carteselectionnee<=0)
		action_carte_precedente->setDisabled(true);
	else
		action_carte_precedente->setDisabled(false);

	if (carteselectionnee>=nbcartes-1 || carteselectionnee>=cartesdebloquees-1)
		action_carte_suivante->setDisabled(true);
	else
		action_carte_suivante->setDisabled(false);
	
	menu_carte->addActions(actions_carte);
	menu_carte->setEnabled(true);
	std::cerr << "BUG? ";
}

void MainWindow_TuxFisher::changerGraphisme(QAction *action)
{
	WTuxFisher->chargerGraphisme(action->text());
}

void MainWindow_TuxFisher::changerSon(QAction *action)
{
	WTuxFisher->chargerSon(action->text());
}

void MainWindow_TuxFisher::changerCarte(QAction *action)
{
	QStringList liste_carte=niveautuxfisher->recupererListeNomCarte();
	for (int i=0; i<liste_carte.count(); i++)
		if (liste_carte.at(i) == action->text())
		{
			niveautuxfisher->selectionnerCarte(i);
			action_carte->setIcon(creerIcone(i));
			chargerCarte();
			break;
		}
}

QStringList MainWindow_TuxFisher::recupererGraphisme()
{
	QString nomrepertoire("graphismes"+(QString)QDir::separator());
	QDir repertoire(nomrepertoire);
	QStringList liste_repertoire;
	liste_repertoire << graphismeParDefaut; // Graphisme par défaut
	liste_repertoire += repertoire.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int i=1; i<liste_repertoire.size(); i++)
	{
		if (!QFileInfo(nomrepertoire+liste_repertoire.at(i)+(QString)QDir::separator()+"avalanche.png").isFile())
		{
			liste_repertoire.removeAt(i);
			i--;
		}
	}
	return liste_repertoire;
}

QStringList MainWindow_TuxFisher::recupererSon()
{
	QString nomrepertoire("sons"+(QString)QDir::separator());
	QDir repertoire(nomrepertoire);
	QStringList liste_repertoire;
	liste_repertoire << aucunSon;
	liste_repertoire += repertoire.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int i=1; i<liste_repertoire.size(); i++)
	{
		if (!QFileInfo(nomrepertoire+liste_repertoire.at(i)+(QString)QDir::separator()+"avalanche.wav").isFile())
		{
			liste_repertoire.removeAt(i);
			i--;
		}
	}
	return liste_repertoire;
}

void MainWindow_TuxFisher::creerMenus()
{
	menu_fichier = menuBar()->addMenu(tr("&Fichier"));
	menu_fichier->addAction(action_chargerfichier);
	menu_fichier->addAction(action_telechargerniveau);
	menu_fichier->addSeparator();
	menu_fichier->addAction(action_quitter);
	
	menu_options = menuBar()->addMenu(tr("&Options"));
	menu_options->addAction(action_musique);
	menu_options_graphisme = new QMenu(tr("Graphismes"),this);
	menu_options_graphisme->setIcon(QIcon(":/ressources/mainwindow/graphisme.png"));
	menu_options_graphisme->addActions(groupeaction_graphisme->actions());
	menu_options->addMenu(menu_options_graphisme);
	
	menu_options_son = new QMenu(tr("Sons"),this);
	menu_options_son->setIcon(QIcon(":/ressources/mainwindow/son.png"));
	menu_options_son->addActions(groupeaction_son->actions());
	menu_options->addMenu(menu_options_son);
	
	menuBar()->addSeparator();
	
	menu_aide = menuBar()->addMenu(tr("&Aide"));
	menu_aide->addAction(action_aide);
	menu_aide->addAction(action_saviezvous);
	menu_aide->addAction(action_apropos);
}

void MainWindow_TuxFisher::creerBarOutils()
{
	baroutil_fichier = addToolBar(tr("&Fichier"));
	baroutil_fichier->setAllowedAreas(Qt::BottomToolBarArea);
	baroutil_fichier->addAction(action_chargerfichier);
	baroutil_fichier->setMovable(false);

	baroutil_carte = addToolBar(tr("&Niveaux"));
	baroutil_carte->addAction(action_carte_precedente);
	baroutil_carte->addAction(action_carte);
	baroutil_carte->addAction(action_carte_suivante);
	baroutil_carte->setMovable(false);
	baroutil_carte->setVisible(false);

	baroutil_tempsecoule = addToolBar(tr("Temp&s écoulé"));

	lcd_tempsecoule = new QLCDNumber(this);
	lcd_tempsecoule->setSegmentStyle(QLCDNumber::Filled);
	lcd_tempsecoule->display("00:00");
	lcd_mouvementeffectue = new QLCDNumber(this);
	lcd_mouvementeffectue->setSegmentStyle(QLCDNumber::Flat);
	lcd_mouvementeffectue->display("0");

	baroutil_tempsecoule->addWidget(lcd_tempsecoule);
	baroutil_tempsecoule->addWidget(lcd_mouvementeffectue);
	baroutil_tempsecoule->setMovable(false);
	baroutil_tempsecoule->setVisible(false);

}

void MainWindow_TuxFisher::creerDocks()
{
	
}

void MainWindow_TuxFisher::creerBarEtat()
{
	msgbaretat = new QLabel(this);
	msgbaretat->setIndent(3);
	
	statusBar()->addWidget(msgbaretat,1);
	statusBar()->setSizeGripEnabled(false);
}

void MainWindow_TuxFisher::afficherMessageBarEtat(QString msg)
{
	msgbaretat->setText(msg);
}

void MainWindow_TuxFisher::cartePrecedente()
{
	int num = niveautuxfisher->recupererNumeroCarte();
	num--;
	if (niveautuxfisher->selectionnerCarte(num))
	{
		chargerCarte();
		action_carte->setIcon(creerIcone(num));
	}
}

void MainWindow_TuxFisher::carteSuivante()
{
	timer_niveautermine->stop();
	int num = niveautuxfisher->recupererNumeroCarte();
	num++;
	if (niveautuxfisher->selectionnerCarte(num))
	{
		chargerCarte();
		action_carte->setIcon(creerIcone(num));
	}
}

void MainWindow_TuxFisher::gameOver()
{
	timer_temps->stop();
	afficherMessageBarEtat(tr("Partie perdue."));
	timer_gameover->start();
}

void MainWindow_TuxFisher::niveauTermine()
{
	timer_temps->stop();
	niveautuxfisher->sauverCartesDebloquees(niveautuxfisher->recupererNumeroCarte()+2);
	majMenuCarte();
	afficherMessageBarEtat(tr("Partie gagnée."));
	timer_niveautermine->start();
}

void MainWindow_TuxFisher::aPropos()
{
	QMessageBox::about(this, tr("A Propos de %1").arg(NOM_APPLICATION),
		tr("<p>%1 - Licence GPL<br/>Copyright &copy; 2008 - Xavier Monin<br/><br/>"
		"Développeur: Xavier Monin<br />"
		"Site Web: <a href=\"http://wwww.tuxfisher.net/\">http://wwww.tuxfisher.net</a><br />"
		"Musique: General Fuzz - Acclimate <a href=\"http://creativecommons.org/licenses/by-nc-nd/2.0/deed.fr\">(Licence Creative Common)</a>").arg(NOM_APPLICATION));
}

void MainWindow_TuxFisher::chargerNiveau()
{
	choixNiveauDialog_TuxFisher choixniveaudialog(this);
	if (choixniveaudialog.exec())
	{
		QString fichier;
		int numerocarte;
		fichier=choixniveaudialog.recupererNomFichier();
		numerocarte = choixniveaudialog.recupererNumeroCarte();
		afficherMessageBarEtat(tr("Chargement en cours..."));
		if (!niveautuxfisher->chargerFichier(fichier))
		{
			afficherMessageBarEtat(tr("Chargement échoué."));
			QMessageBox::information(this,tr("Chargement de la partie impossible"),
				tr("La partie n'a pas pu être chargée"), QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		niveautuxfisher->selectionnerCarte(numerocarte);
		chargerCarte();
		action_carte->setIcon(creerIcone(numerocarte));
		baroutil_carte->setVisible(true);
		baroutil_tempsecoule->setVisible(true);

		changerTitreFenetre(true);

		afficherMessageBarEtat(tr("Chargement de la partie terminé."));
		
		if (!isMaximized())
			adjustSize();
	}
}

void MainWindow_TuxFisher::telechargerNiveau()
{
	telechargerNiveauDialog telechargerniveaudialog(this);
	telechargerniveaudialog.exec();
}

void MainWindow_TuxFisher::closeEvent(QCloseEvent *event)
{
	int reponse=QMessageBox::warning(this, NOM_APPLICATION,
                        tr("Etes vous sûr de vouloir quitter %1?").arg(NOM_APPLICATION),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No);
	if (reponse==QMessageBox::Yes)
	{
		ecrireConfiguration();
		event->accept();
	}
	else
		event->ignore();
}
