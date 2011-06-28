#include "Repository.h"
#include <QDebug>
#include "KissArchive.h"
#include "MainWindow.h"
#include <QCoreApplication>

#define TYPE_AVAIL 	1001
#define TYPE_INSTALLED 	1002

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
	MainWindow::ref().setTabName(this, tr("Repository"));
	foreach(const QString& name, KissArchive::installed()) {
		ui_installList->addItem(new QListWidgetItem(name, ui_installList, TYPE_INSTALLED));
	}
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
	ui_stagedList->addItem(new QListWidgetItem(QIcon(":/shortcuts/target/icon_set/icons/tick.png"), item->text(), ui_stagedList, item->type()));
	delete ui_list->takeItem(ui_list->row(item));
}

void Repository::on_ui_unmark_clicked()
{
	QListWidgetItem* item = ui_stagedList->currentItem();
	if(!item) return;
	if(item->type() == TYPE_AVAIL) ui_list->addItem(new QListWidgetItem(item->text(), ui_list, item->type()));
	else ui_installList->addItem(new QListWidgetItem(item->text(), ui_installList, item->type()));
	delete ui_stagedList->takeItem(ui_stagedList->row(item));
}

void Repository::on_ui_uninstall_clicked()
{
	QListWidgetItem* item = ui_installList->currentItem();
	if(!item) return;
	ui_stagedList->addItem(new QListWidgetItem(QIcon(":/shortcuts/file/icon_set/icons/cross.png"), item->text(), ui_stagedList, item->type()));
	delete ui_installList->takeItem(ui_installList->row(item));
}

void Repository::on_ui_begin_clicked()
{
	disconnect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
	connect(&m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	next();
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
		"win"
#else
		"nix"
#endif
	);
	foreach(const QString& line, lines) {
		ui_list->addItem(new QListWidgetItem(line.section("\t", 1, 1) + line.section("\t", 2, 2), ui_list, TYPE_AVAIL));
		m_locations[line.section("\t", 1, 1)] = line.section("\t", 3, 3);
	}
}

void Repository::downloadFinished(QNetworkReply* reply)
{
	disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
	if(!KissArchive::install(reply)) {
		qWarning() << "Installing" << reply->request().url() << "failed!";
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
		ui_log->addItem(tr("Closing KISS..."));
		QCoreApplication::exit();

		return;
	}
	
	if(item->type() == TYPE_INSTALLED) {
		ui_log->addItem(tr("Uninstalling ") + item->text() + "...");
		KissArchive::uninstall(item->text());
		next();
		return;
	}
	
	ui_log->addItem(tr("Downloading ") + item->text() + "...");
	
	QNetworkReply* reply = m_network.get(QNetworkRequest(QUrl("http://files.kipr.org/kiss/" + m_locations[item->text()] + "/package.kiss")));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}