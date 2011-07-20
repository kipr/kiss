/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#ifndef __REPOSITORY_H__
#define __REPOSITORY_H__

#include "ui_Repository.h"
#include "Tab.h"

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Repository : public QWidget, public Tab, private Ui::Repository
{
	Q_OBJECT
public:
	Repository(QWidget* parent = 0);
	
	void activate();
	void addActionsFile(QMenu* file);
	void addActionsEdit(QMenu* edit);
	void addActionsHelp(QMenu* help);
	void addOtherActions(QMenuBar* menuBar);
	void addToolbarActions(QToolBar* toolbar);
	
	bool beginSetup();
	void completeSetup();
	
	bool close();
	
	void moveTo(int line, int pos);
	
public slots:
	void refreshSettings();
	
private slots:
	void on_ui_mark_clicked();
	void on_ui_unmark_clicked();
	void on_ui_uninstall_clicked();
	void on_ui_begin_clicked();
	void on_ui_source_clicked();
	
	void downloadProgress(qint64, qint64);

	void finished(QNetworkReply* reply);
	void downloadFinished(QNetworkReply* reply);
private:
	void next();
	
	QNetworkAccessManager m_network;
	QMap<QString, QString> m_locations;
	QString m_source;
};

#endif
