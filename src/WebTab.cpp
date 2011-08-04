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
#include "VideoPlayerTab.h"

#include <QUrl>
#include <QToolBar>
#include <QWebHistory>
#include <QDesktopServices>
#include <QDebug>
#include <QDir>

WebTab::WebTab(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	
	connect(ui_webView, SIGNAL(titleChanged(const QString&)), this, SLOT(updateTitle(const QString&)));
	connect(ui_webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(updateUrl(const QUrl&)));
	
	webView()->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); // Make sure we get to handle special links
	connect(webView(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));
	
	ui_frameFind->hide();
}

void WebTab::activate() {}
void WebTab::addActionsFile(QMenu*) {}

void WebTab::addActionsEdit(QMenu* edit)
{	
	edit->addAction(actionCopy);
	edit->addAction(actionCut);
	edit->addAction(actionPaste);
	edit->addSeparator();
	edit->addAction(actionFind);
}

void WebTab::addActionsHelp(QMenu*) {}

void WebTab::addOtherActions(QMenuBar* menuBar)
{
	QMenu* browser = menuBar->addMenu(tr("Browser"));
	browser->addAction(actionBack);
	browser->addAction(actionForward);
	browser->addAction(actionReload);
	browser->addAction(actionGo);
	browser->addAction(actionOpenInBrowser);
}

void WebTab::addToolbarActions(QToolBar* toolbar)
{
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addAction(actionFind);
	toolbar->addSeparator();
	toolbar->addAction(actionBack);
	toolbar->addAction(actionForward);
	toolbar->addAction(actionReload);
	toolbar->addAction(actionGo);
	toolbar->addAction(actionOpenInBrowser);
	
}

bool WebTab::beginSetup() { return true; }

void WebTab::completeSetup()
{
	updateTitle("");
	updateUrl(QUrl::fromUserInput("about:blank"));
}

void WebTab::updateTitle(const QString& title)
{
	QString local = title;
	if(title.length() > 40) {
		local.truncate(40);
		local += "...";
	}
	MainWindow::ref().setTabName(this, local.length() == 0 ? tr("Untitled Browser") : local);
}

void WebTab::updateUrl(const QUrl& url)
{
	actionBack->setEnabled(ui_webView->history()->canGoBack());
	actionForward->setEnabled(ui_webView->history()->canGoForward());
	
	ui_urlBar->setText(url.toString());
	MainWindow::ref().setStatusMessage(tr("Loaded ") + url.toString(), 5000);
}

void WebTab::on_actionGo_triggered() { ui_webView->load(QUrl::fromUserInput(ui_urlBar->text())); }
void WebTab::on_actionCopy_triggered() { ui_webView->triggerPageAction(QWebPage::Copy); }
void WebTab::on_actionCut_triggered() { ui_webView->triggerPageAction(QWebPage::Cut); }
void WebTab::on_actionPaste_triggered() { ui_webView->triggerPageAction(QWebPage::Paste); }
bool WebTab::close() { return true; }

void WebTab::load(QString url, bool hideUrl)
{
	ui_webView->load(QUrl::fromUserInput(url));
	actionGo->setEnabled(!hideUrl);
	ui_urlBar->setVisible(!hideUrl);
	ui_goButton->setVisible(!hideUrl);
}

void WebTab::on_ui_prevFind_clicked() { ui_webView->findText(ui_find->text(), QWebPage::FindBackward); }
void WebTab::on_ui_nextFind_clicked() { ui_webView->findText(ui_find->text()); }

void WebTab::on_ui_webView_loadFinished(bool ok)
{
	ui_load->hide();
	if(!ok) ui_webView->setHtml("Error loading page");
}

void WebTab::on_actionOpenInBrowser_triggered() { QDesktopServices::openUrl(ui_webView->url()); }

void WebTab::refreshSettings() {}
QWebView* WebTab::webView() { return ui_webView; }

void WebTab::linkClicked(const QUrl& url)
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
	if(auth == "settings") {
		MainWindow::ref().on_actionEditor_Settings_triggered();
		return;
	}
	
	QString fragment = url.fragment();
	fragment.replace("KISS_CWD", QDir::currentPath());
	qWarning() << fragment;
	if(auth == "newbrowser") {
		WebTab* tab = new WebTab();
		tab->load(fragment);
		MainWindow::ref().addTab(tab);
		return;
	}
	if(auth == "openfile") {
		MainWindow::ref().openFile(fragment);
		return;
	}
	if(auth == "video") {
		VideoPlayerTab* tab = new VideoPlayerTab();
		tab->load(fragment);
		MainWindow::ref().addTab(tab);
		return;
	}
	if(auth == "external") {
		QDesktopServices::openUrl(QUrl::fromUserInput(fragment));	
		return;
	}
}
