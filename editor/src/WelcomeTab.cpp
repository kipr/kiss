#include "WelcomeTab.h"
#include <QPalette>
#include <QFile>
#include <QDebug>
#include <QWebPage>
#include "MainWindow.h"

WelcomeTab::WelcomeTab(MainWindow* mainWindow) : WebTab(mainWindow)
{
	
}	

void WelcomeTab::completeSetup()
{	
	WebTab::completeSetup(); 
	load("", true); // Hide URL Bar
	webView()->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); // Make sure we get to handle special links
	connect(webView(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));
	
	QFile file(":/welcome/index.html");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QByteArray data = file.readAll();
	data.replace("KISS_BACKGROUND", m_mainWindow->tabWidget()->palette().color(QPalette::Background).name().toAscii());
	
	// qWarning() << data.data();
	
	webView()->setHtml(data.data());
}

void WelcomeTab::linkClicked(const QUrl& url)
{
	if(url.scheme() != "kiss") load(url.toString());
	QString auth = url.authority();
	if(auth == "new") {
		m_mainWindow->newFile();
		return;
	}
	if(auth == "open") {
		m_mainWindow->on_actionOpen_triggered();
		return;
	}
	if(auth == "newbrowser") {
		qWarning() << "New Browser!";
		WebTab* tab = new WebTab(m_mainWindow);
		qWarning() << url.fragment();
		tab->load(url.fragment());
		m_mainWindow->addTab(tab);
		return;
	}
	if(auth == "help") {
		// No such thing as help yet :P
		return;
	}
}