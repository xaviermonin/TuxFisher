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

#include <QtGui>
#include "saviezvousdialog.h"

saviezVousDialog::saviezVousDialog(QWidget *parent)
: QDialog(parent)
{
	setWindowTitle(tr("Le saviez-vous ?"));
	//setMinimumWidth(400);

	liste << tr("<center><b>Bienvenue sur TuxFisher</b></center><br/>"
				"TuxFisher est un jeu de réflexion ou vous devez manger les poissons<br/>"
				"sans se faire écraser par les avalanches et sans se retrouver bloqué.<br/>"
				"Utilisez les touches directionnelles (haut, bas, gauche, droite) pour vous déplacer.<br/>"
				"La touche [Ret. Arrière] (au dessus de [Entrer]) permet d'annuler un déplacement.<br/>"
				"Amusez-vous bien.");
	liste << tr("Si vous avez fait un mouvement que vous souhaitez annuler,\n"
				"appuyez sur la touche [Ret. Arrière] (au dessus de [Entrer])\n"
				"et vous reviendrez à votre ancienne position.\n"
				"Vous pouvez annuler jusqu'à 20 déplacements.");
	liste << tr("TuxFisher est un clone libre et gratuit du jeu Mr Matt, \n"
				"développé par J. Andrzej Wrotniak en 1996.\n"
				"Mr Matt et TuxFisher sont des dérivés de Boulder Dash");
	liste << tr("Vous pouvez activer et désactiver la musique du jeu par le menu Options.\n"
				"Le titre de la musique du jeu est Acclimate de General Fuzz.\n");
	liste << tr("Vous aussi vous pouvez créer des jeux pour TuxFisher \n"
				"grâce à l'éditeur de jeu de TuxFisher.\n"
				"Une fois terminé, vous pouvez mettre en ligne vos créations\n"
				"et les partager avec la communauté.");
	liste << tr("Vous pouvez choisir les graphismes par le menu Options->Graphismes.\n"
				"Vous pouvez créer vos propres graphismes en créant un répertoire\n"
				"avec le nom désiré dans le répertoire \"graphismes\" du jeu.\n"
				"Ce répertoire doit contenir les images pour chaque case.");
	liste << tr("Vous pouvez choisir les bruitages par le menu Options->Sons\n"
				"Pour désactiver les bruitages: Options->Sons->Aucun Son\n"
				"Vous pouvez créer vos bruitages en créant un nouveau répertoire\n"
				"dans le répertoire \"sons\" du jeu.");

	bouton_fermer = new QPushButton(tr("Fermer"), this);
	bouton_suivant = new QPushButton(tr("Suivant"), this);
	bouton_precedent = new QPushButton(tr("Précédent"), this);

	checkbox_afficher = new QCheckBox(tr("Ne plus afficher"), this);

	label_image = new QLabel(this);
	label_image->setPixmap(QPixmap(":/ressources/mainwindow/saviezvous.png"));

	label_texte = new QLabel(this);
	//label_texte->setWordWrap(true);

	qsrand(QDateTime::currentDateTime().toTime_t());
	index=(qrand()%(liste.count()-1))+1;
	chargerLabel();
	//label->setAlignment(Qt::AlignCenter);

	QHBoxLayout *layout_labels = new QHBoxLayout;
	layout_labels->addWidget(label_texte);
	layout_labels->addStretch();
	layout_labels->addWidget(label_image);

	QHBoxLayout *layout_boutons = new QHBoxLayout;
	layout_boutons->addWidget(checkbox_afficher);
	layout_boutons->addStretch();
	layout_boutons->addWidget(bouton_fermer);
	layout_boutons->addWidget(bouton_precedent);
	layout_boutons->addWidget(bouton_suivant);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(layout_labels);
	layout->addLayout(layout_boutons);
	setLayout(layout);

	connect( bouton_fermer, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( bouton_suivant, SIGNAL(clicked()), this, SLOT(suivant()) );
	connect( bouton_precedent, SIGNAL(clicked()), this, SLOT(precedent()) );
}

saviezVousDialog::~saviezVousDialog()
{

}

void saviezVousDialog::afficherPresentation()
{
	index=0;
	chargerLabel();
}

void saviezVousDialog::chargerLabel()
{
	label_texte->setText(liste.at(index));

	if (index<=0)
		bouton_precedent->setDisabled(true);
	else
		bouton_precedent->setDisabled(false);

	if (index>=liste.count()-1)
		bouton_suivant->setDisabled(true);
	else
		bouton_suivant->setDisabled(false);
	adjustSize();
}

void saviezVousDialog::precedent()
{
	if (index>0)
		index--;
	chargerLabel();
}

void saviezVousDialog::suivant()
{
	if (index<liste.count()-1)
		index++;
	chargerLabel();
}
