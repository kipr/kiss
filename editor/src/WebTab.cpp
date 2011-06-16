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

#include "WebTab.h"
#include "MainWindow.h"
#include <QUrl>
#include <QToolBar>
#include <QWebHistory>

WebTab::WebTab(MainWindow* mainWindow) : Tab(mainWindow)
{
	setupUi(this);
	
	connect(ui_webView, SIGNAL(titleChanged(QString)), this, SLOT(updateTitle(QString)));
	connect(ui_webView, SIGNAL(urlChanged(QUrl)), this, SLOT(updateUrl(QUrl)));
}

WebTab::~WebTab()
{	
}

void WebTab::addActionsFile(QMenu* file)
{	
}

void WebTab::addActionsEdit(QMenu* edit)
{	
	edit->addAction(actionCopy);
	edit->addAction(actionCut);
	edit->addAction(actionPaste);
}

void WebTab::addActionsHelp(QMenu* help)
{
}

void WebTab::addOtherActions(QMenuBar* menuBar)
{
	QMenu* browser = menuBar->addMenu("Browser");
	browser->addAction(actionBack);
	browser->addAction(actionForward);
	browser->addAction(actionReload);
	browser->addAction(actionGo);
}

void WebTab::addToolbarActions(QToolBar* toolbar)
{
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addSeparator();
	toolbar->addAction(actionBack);
	toolbar->addAction(actionForward);
	toolbar->addAction(actionReload);
	toolbar->addAction(actionGo);
}

bool WebTab::beginSetup()
{
}

void WebTab::completeSetup()
{
	updateTitle("");
	updateUrl(QUrl("about:blank"));
}

void WebTab::updateTitle(QString title)
{
	if(title.length() > 40) {
		title.truncate(40);
		title += "...";
	}
	m_mainWindow->setTabName(this, title.length() == 0 ? "Untitled Browser" : title);
}

void WebTab::updateUrl(QUrl url)
{
	actionBack->setEnabled(ui_webView->history()->canGoBack());
	actionForward->setEnabled(ui_webView->history()->canGoForward());
	
	ui_urlBar->setText(url.toString());
	m_mainWindow->setStatusMessage("Loaded " + url.toString(), 5000);
}

void WebTab::on_actionGo_triggered()
{
	ui_webView->load(QUrl(ui_urlBar->text()));
}

void WebTab::on_actionCopy_triggered()
{
	ui_webView->triggerPageAction(QWebPage::Copy);
}

void WebTab::on_actionCut_triggered()
{
	ui_webView->triggerPageAction(QWebPage::Cut);
}

void WebTab::on_actionPaste_triggered()
{
	ui_webView->triggerPageAction(QWebPage::Paste);
}

bool WebTab::close()
{
	return true;
}

void WebTab::load(QString url)
{
	ui_webView->load(QUrl(url));
}

void WebTab::moveTo(int line, int pos)
{
	
}

void WebTab::refreshSettings()
{
}
