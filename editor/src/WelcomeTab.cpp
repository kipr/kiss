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
	
	qWarning() << data.data();
	
	webView()->setHtml(data.data());
}

void WelcomeTab::linkClicked(const QUrl& url)
{
	qWarning() << "Link " << url << "clicked!";
}