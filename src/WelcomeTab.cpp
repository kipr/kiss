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

#include "WelcomeTab.h"
#include <QPalette>
#include <QFile>
#include <QDebug>
#include <QWebPage>
#include "MainWindow.h"

WelcomeTab::WelcomeTab(QWidget* parent) : WebTab(parent)
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
	data.replace("KISS_BACKGROUND", palette().color(QPalette::Background).name().toAscii());
	
	webView()->setHtml(data.data());
	
	actionOpenInBrowser->setEnabled(false);
}

void WelcomeTab::linkClicked(const QUrl& url)
{
	if(url.scheme() != "kiss") load(url.toString());
	QString auth = url.authority();
	if(auth == "new") {
		MainWindow::ref().newFile();
		return;
	}
	if(auth == "open") {
		MainWindow::ref().on_actionOpen_triggered();
		return;
	}
	if(auth == "newbrowser") {
		WebTab* tab = new WebTab();
		qWarning() << url.fragment();
		tab->load(url.fragment());
		MainWindow::ref().addTab(tab);
		return;
	}
	if(auth == "help") {
		return;
	}
}