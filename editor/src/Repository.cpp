#include "Repository.h"
#include <QDebug>
#include "KissArchive.h"
#include "MainWindow.h"

Repository::Repository(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

void Repository::addActionsFile(QMenu* file)
{
	
}

void Repository::addActionsEdit(QMenu* edit)
{
	
}

void Repository::addActionsHelp(QMenu* help)
{
	
}

void Repository::addOtherActions(QMenuBar* menuBar)
{
	
}

void Repository::addToolbarActions(QToolBar* toolbar)
{
	
}

bool Repository::beginSetup()
{
	return true;
}

void Repository::completeSetup()
{
	m_network.get(QNetworkRequest(QUrl("http://files.kipr.org/kiss/available.lst")));
	MainWindow::ref().setTabName(this, "Repository");
}

bool Repository::close()
{
	return true;
}

void Repository::moveTo(int line, int pos)
{
	
}

void Repository::refreshSettings()
{
	
}

void Repository::on_ui_mark_clicked()
{
	QListWidgetItem* item = ui_list->currentItem();
	if(!item) return;
	ui_installList->addItem(item->text());
	ui_list->takeItem(ui_list->row(item));
}

void Repository::on_ui_unmark_clicked()
{
	QListWidgetItem* item = ui_installList->currentItem();
	if(!item) return;
	ui_list->addItem(item->text());
	ui_installList->takeItem(ui_installList->row(item));
}

void Repository::on_ui_begin_clicked()
{
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
	connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	downloadNext();
}

void Repository::finished(QNetworkReply* reply) 
{
	if(reply->error() != QNetworkReply::NoError) {
		qWarning() << "Error:" << reply->error();
		return; 
	}
	ui_list->clear();
	m_locations.clear();
	QStringList lines = QString(reply->readAll()).split('\n');
	lines = lines.filter(
#ifdef Q_OS_MAC
		"mac"
#elif defined(Q_OS_WIN)
		"windows"
#else
		"unix"
#endif
	);
	foreach(const QString& line, lines) {
		ui_list->addItem(line.section("\t", 1, 1));
		m_locations[line.section("\t", 1, 1)] = line.section("\t", 2, 2);
	}
}

void Repository::downloadFinished(QNetworkReply* reply)
{
	disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
	if(!KissArchive::extract(reply)) {
		qWarning() << "Installing" << reply->request().url() << "failed!";
	}
	downloadNext();
}

void Repository::downloadProgress(qint64 finished, qint64 total)
{
	ui_progressBar->setMinimum(0);
	ui_progressBar->setMaximum(total);
	ui_progressBar->setValue(finished);
}

void Repository::downloadNext()
{
	QListWidgetItem* item = ui_installList->takeItem(0);
	ui_progressBar->setEnabled(item != 0);
	ui_mark->setEnabled(item == 0);
	ui_unmark->setEnabled(item == 0);
	ui_begin->setEnabled(item == 0);
	
	if(!item) return;
	
	QNetworkReply* reply = m_network.get(QNetworkRequest(QUrl("http://files.kipr.org/kiss/" + m_locations[item->text()] + "/package.kiss")));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}