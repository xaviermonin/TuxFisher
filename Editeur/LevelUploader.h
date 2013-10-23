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

#ifndef _LEVEL_UPLOADER_H_
#define _LEVEL_UPLOADER_H_

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QHttp>
#include <QProgressBar>

class LevelUploader : public QDialog
{
	Q_OBJECT
public:
	LevelUploader(QWidget *parent=0);
	~LevelUploader();
	void loadFileName(QString);
private:
	bool overwrite;
	QString fullfilename;
	QString filename;
	QString host;
	QString destinationupload;
	
	QLabel *label_namefile;
	QProgressBar *progress_bar;
	
	QGridLayout *group;
	QHBoxLayout *layout_button;
	QVBoxLayout *layout;
	
	QPushButton *buttonsend;
//	QPushButton *buttonclose;

	QHttp *http;
private slots:
	void sendfile();
	void setProgressBar(int, int);
	void checkRequest(bool);
};

#endif // !_METTREENLIGNEDIALOG_EDITEUR_H_
