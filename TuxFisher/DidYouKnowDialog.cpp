/*
Projet: TuxFisher
Licence: GPLv2
Version: BETA
Author: Xavier Monin
Date: 03/06/2010
Site: http://www.tuxfisher.net
Mail: tuxfisher@free.fr
Description: Jeu d'arcade/reflexion 2D mettant en sc�ne le personnage Tux
Copyright 2010 - Xavier Monin - Tous droits r�serv�s
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
#include "DidYouKnowDialog.h"

DidYouKnowDialog::DidYouKnowDialog(QWidget *parent)
: QDialog(parent)
{
	setWindowTitle(tr("Le saviez-vous ?"));
	//setMinimumWidth(400);

	list << tr("<center><b>Bienvenue sur TuxFisher</b></center><br/>"
				"TuxFisher est un jeu de r�flexion ou vous devez manger les fishs<br/>"
				"sans se faire �craser par les avalanches et sans se retrouver bloqu�.<br/>"
				"Utilisez les touches directionnelles (up, down, gauche, droite) pour vous d�placer.<br/>"
				"La touche [Ret. Arri�re] (au dessus de [Entrer]) permet d'annuler un d�placement.<br/>"
				"Amusez-vous bien.");
	list << tr("Si vous avez fait un mouvement que vous souhaitez annuler,\n"
				"appuyez sur la touche [Ret. Arri�re] (au dessus de [Entrer])\n"
				"et vous reviendrez � votre ancienne position.\n"
				"Vous pouvez annuler jusqu'� 20 d�placements.");
	list << tr("TuxFisher est un clone libre et gratuit du jeu Mr Matt, \n"
				"d�velopp� par J. Andrzej Wrotniak en 1996.\n"
				"Mr Matt et TuxFisher sont des d�riv�s de Boulder Dash");
	list << tr("Vous pouvez activer et d�sactiver la music du jeu par le menu Options.\n"
				"Le titre de la music du jeu est Acclimate de General Fuzz.\n");
	list << tr("Vous aussi vous pouvez cr�er des jeux pour TuxFisher \n"
				"gr�ce � l'�diteur de jeu de TuxFisher.\n"
				"Une fois termin�, vous pouvez mettre en ligne vos cr�ations\n"
				"et les partager avec la communaut�.");
	list << tr("Vous pouvez choisir les graphismes par le menu Options->Graphisms.\n"
				"Vous pouvez cr�er vos propres graphismes en cr�ant un r�pertoire\n"
				"avec le nom d�sir� dans le r�pertoire \"graphismes\" du jeu.\n"
				"Ce r�pertoire doit contenir les images pour chaque case.");
	list << tr("Vous pouvez choisir les bruitages par le menu Options->Sons\n"
				"Pour d�sactiver les bruitages: Options->Sons->Aucun Son\n"
				"Vous pouvez cr�er vos bruitages en cr�ant un new r�pertoire\n"
				"dans le r�pertoire \"sons\" du jeu.");

	button_close = new QPushButton(tr("Fermer"), this);
	button_next = new QPushButton(tr("Suivant"), this);
	button_previous = new QPushButton(tr("Pr�c�dent"), this);

	checkbox_afficher = new QCheckBox(tr("Ne plus afficher"), this);

	label_image = new QLabel(this);
	label_image->setPixmap(QPixmap(":/ressources/mainwindow/saviezvous.png"));

	label_text = new QLabel(this);
	//label_text->setWordWrap(true);

	qsrand(QDateTime::currentDateTime().toTime_t());
	index=(qrand()%(list.count()-1))+1;
	loadLabel();
	//label->setAlignment(Qt::AlignCenter);

	QHBoxLayout *layout_labels = new QHBoxLayout;
	layout_labels->addWidget(label_text);
	layout_labels->addStretch();
	layout_labels->addWidget(label_image);

	QHBoxLayout *layout_buttons = new QHBoxLayout;
	layout_buttons->addWidget(checkbox_afficher);
	layout_buttons->addStretch();
	layout_buttons->addWidget(button_close);
	layout_buttons->addWidget(button_previous);
	layout_buttons->addWidget(button_next);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(layout_labels);
	layout->addLayout(layout_buttons);
	setLayout(layout);

	connect( button_close, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( button_next, SIGNAL(clicked()), this, SLOT(next()) );
	connect( button_previous, SIGNAL(clicked()), this, SLOT(previous()) );
}

DidYouKnowDialog::~DidYouKnowDialog()
{

}

void DidYouKnowDialog::displayWelcome()
{
	index=0;
	loadLabel();
}

void DidYouKnowDialog::loadLabel()
{
	label_text->setText(list.at(index));

	if (index<=0)
		button_previous->setDisabled(true);
	else
		button_previous->setDisabled(false);

	if (index>=list.count()-1)
		button_next->setDisabled(true);
	else
		button_next->setDisabled(false);
	adjustSize();
}

void DidYouKnowDialog::previous()
{
	if (index>0)
		index--;
	loadLabel();
}

void DidYouKnowDialog::next()
{
	if (index<list.count()-1)
		index++;
	loadLabel();
}
