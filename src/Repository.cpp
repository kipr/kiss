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

#include "Repository.h"
#include <QDebug>
#include "KissArchive.h"
#include "MainWindow.h"
#include "Kiss.h"
#include "TargetManager.h"
#include "LexerSpecManager.h"
#include "SourceDialog.h"

#include <QCoreApplication>

#define DELIM "\t"

#define TYPE_AVAIL 	1001
#define TYPE_INSTALLED 	1002

Repository::Repository(QWidget* parent) : QWidget(parent), m_source("http://files.kipr.org/kiss/")
{
	setupUi(this);
}

Repository::~Repository() {}

void Repository::activate() 
{
	MainWindow::ref().setTitle(m_source);
}

void Repository::addActionsFile(QMenu* file) { Q_UNUSED(file); }
void Repository::addActionsEdit(QMenu* edit) { Q_UNUSED(edit); }
void Repository::addActionsHelp(QMenu* help) { Q_UNUSED(help); }
void Repository::addOtherActions(QMenuBar* menuBar) { Q_UNUSED(menuBar); }
void Repository::addToolbarActions(QToolBar* toolbar) { Q_UNUSED(toolbar); }
bool Repository::beginSetup() { return true; }

void Repository::completeSetup()
{
	MainWindow::ref().setTabName(this, tr("Repository"));
	
	// Just in case this isn't the first call to completeSetup
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
	
	connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
	
	m_network.get(QNetworkRequest(QUrl(m_source + "available.lst")));
	
	ui_list->clear();
	m_locations.clear();
	ui_installList->clear();
	foreach(const QString& name, KissArchive::installed()) {
		QListWidgetItem* item = new QListWidgetItem(name + " - v" + QString::number(KissArchive::version(name)), ui_installList, TYPE_INSTALLED);
		item->setData(Qt::UserRole, name);
		ui_installList->addItem(item);
	}
	
	ui_availGroup->setTitle(m_source + "available.lst");
	MainWindow::ref().setTitle(m_source);
}

bool Repository::close() { return true; }

void Repository::moveTo(int line, int pos)
{
	Q_UNUSED(line)
	Q_UNUSED(pos)
}

void Repository::refreshSettings() {}

void Repository::on_ui_mark_clicked()
{
	QListWidgetItem* item = ui_list->currentItem();
	if(!item) return;
	QListWidgetItem* nItem = new QListWidgetItem(QIcon(":/shortcuts/target/icon_set/icons/tick.png"), item->text(), ui_stagedList, TYPE_AVAIL);
	nItem->setData(Qt::UserRole, item->data(Qt::UserRole));
	ui_stagedList->addItem(nItem);
	delete ui_list->takeItem(ui_list->row(item));
}

void Repository::on_ui_unmark_clicked()
{
	QListWidgetItem* item = ui_stagedList->currentItem();
	if(!item) return;
	QListWidgetItem* nItem = new QListWidgetItem(item->text(), ui_list, item->type());
	nItem->setData(Qt::UserRole, item->data(Qt::UserRole));
	if(item->type() == TYPE_AVAIL) ui_list->addItem(nItem);
	else ui_installList->addItem(nItem);
	
	delete ui_stagedList->takeItem(ui_stagedList->row(item));
}

void Repository::on_ui_uninstall_clicked()
{
	QListWidgetItem* item = ui_installList->currentItem();
	if(!item) return;
	QListWidgetItem* nItem = new QListWidgetItem(QIcon(":/shortcuts/file/icon_set/icons/cross.png"), item->text(), ui_stagedList, TYPE_INSTALLED);
	nItem->setData(Qt::UserRole, item->data(Qt::UserRole));
	ui_stagedList->addItem(nItem);
	delete ui_installList->takeItem(ui_installList->row(item));
}

void Repository::on_ui_begin_clicked()
{
	MainWindow::ref().closeAllOthers(this);
	TargetManager::ref().unloadAll();
	LexerSpecManager::ref().unloadAll();
	
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
	connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	next();
}

void Repository::on_ui_source_clicked()
{
	SourceDialog sourceDialog(this);
	sourceDialog.setSource(m_source);
	if(sourceDialog.exec() == QDialog::Rejected) return;
	m_source = sourceDialog.source();
	completeSetup();
}

void Repository::finished(QNetworkReply* reply) 
{
	ui_list->clear();
	m_locations.clear();
	if(reply->error() != QNetworkReply::NoError) {
		ui_log->addItem(tr("Error fetching list (") + QString::number(reply->error()) + tr(") from ") + m_source);
		return; 
	}
	QStringList lines = QString(reply->readAll()).split('\n');
	lines = lines.filter(OS_NAME);
	foreach(const QString& line, lines) {
		QString name = line.section(DELIM, 1, 1) + " - v" + line.section(DELIM, 2, 2);
		ui_list->addItem(new QListWidgetItem(name, ui_list, TYPE_AVAIL));
		qWarning() << line.section(DELIM, 1, 1) << line.section(DELIM, 2, 2) << line.section(DELIM, 3, 3);
		m_locations[name] = line.section(DELIM, 3, 3);
	}
}

void Repository::downloadFinished(QNetworkReply* reply)
{
	if(reply->error() != QNetworkReply::NoError) {
		ui_log->addItem(tr("Error fetching (") + QString::number(reply->error()) + tr(") from ") + reply->request().url().toString());
		next();
		return; 
	}
	
	disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
	ui_log->addItem(tr("Installing ") + reply->request().url().toString() + "...");
	KissReturn ret(KissArchive::install(reply));
	if(ret.error) {
		ui_log->addItem(ret.message);
		ui_log->addItem(tr("Install ") + reply->request().url().toString() + " FAILED");
	}
	next();
}

void Repository::downloadProgress(qint64 finished, qint64 total)
{
	ui_progressBar->setMinimum(0);
	ui_progressBar->setMaximum(total);
	ui_progressBar->setValue(finished);
}

void Repository::next()
{
	QListWidgetItem* item = ui_stagedList->takeItem(0);
	ui_progressBar->setEnabled(item != 0);
	ui_mark->setEnabled(item == 0);
	ui_unmark->setEnabled(item == 0);
	ui_list->setEnabled(item == 0);
	ui_installList->setEnabled(item == 0);
	ui_stagedList->setEnabled(item == 0);
	ui_source->setEnabled(item == 0);
	ui_uninstall->setEnabled(item == 0);
	ui_begin->setEnabled(item == 0);
	
	if(!item) {
		ui_log->addItem(tr("Complete!"));
		ui_log->addItem(tr("Please restart KISS."));
		completeSetup();
		return;
	}
	
	if(item->type() == TYPE_INSTALLED) {
		ui_log->addItem(tr("Uninstalling ") + item->text() + "...");
		KissReturn ret(KissArchive::uninstall(item->data(Qt::UserRole).toString()));
		if(ret.error) ui_log->addItem(ret.message);
		next();
		return;
	}
	
	ui_log->addItem(tr("Downloading ") + item->text() + "...");
	
	QNetworkReply* reply = m_network.get(QNetworkRequest(QUrl(m_source + m_locations[item->text()])));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}