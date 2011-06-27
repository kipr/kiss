#ifndef __REPOSITORY_H__
#define __REPOSITORY_H__

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "ui_Repository.h"
#include "Tab.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

class Repository : public QWidget, public Tab, private Ui::Repository
{
	Q_OBJECT
public:
	Repository(QWidget* parent = 0);
	
	void addActionsFile(QMenu* file);
	void addActionsEdit(QMenu* edit);
	void addActionsHelp(QMenu* help);
	void addOtherActions(QMenuBar* menuBar);
	void addToolbarActions(QToolBar* toolbar);
	
	bool beginSetup();
	void completeSetup();
	
	bool close();
	
	void moveTo(int line, int pos);
	void refreshSettings();
	
private slots:
	void on_ui_mark_clicked();
	void on_ui_unmark_clicked();
	void on_ui_begin_clicked();
	
	void downloadProgress(qint64, qint64);

	void finished(QNetworkReply* reply);
	void downloadFinished(QNetworkReply* reply);
private:
	void downloadNext();
	
	QNetworkAccessManager m_network;
	QMap<QString, QString> m_locations;	
};

#endif
