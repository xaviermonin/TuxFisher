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

#ifndef _LEVEL_DOWNLOADER_DIALOG_H_
#define _LEVEL_DOWNLOADER_DIALOG_H_

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QLayout>
#include <QBuffer>
#include <QHttp>
#include <QProgressBar>
#include <QTextEdit>
#include <QProgressDialog>


class LevelDownloaderModel;

class LevelDownloaderDialog : public QDialog
{
	Q_OBJECT
public:
	LevelDownloaderDialog(QWidget *parent=0);
	~LevelDownloaderDialog();
private:
	void configurerModele();
	void configurerVue();
	void sendRequete();

	QTableView *vue;
	QStandardItemModel *modele;

	QProgressDialog *dialogue_progress_bar;

	QHttp *http_list;
	QHttp *http_level;
	QBuffer buffer_list;
	QBuffer buffer_level;

	QPushButton *button_teleload;

	QStringList stringlist_id;
	QStringList stringlist_numeroversion;

	int telechargementencours;

private slots:
	void loadModele(bool);
	void downloadLevel();
	void telechargementTermine(bool error);
	void setProgressBar(int, int);
};

#endif // !_LEVEL_DOWNLOADER_DIALOG_H_
