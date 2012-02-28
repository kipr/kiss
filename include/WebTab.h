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

#ifndef __WEB_TAB_H__
#define __WEB_TAB_H__

#include "Tab.h"
#include "ui_WebTab.h"
#include "MacroString.h"
#include "AudioTutorial.h"

#define KISS_BACKGROUND "KISS_BACKGROUND"

class MainWindow;

class WebTab : public QWidget, public TabbedWidget, protected Ui::WebTab
{
	Q_OBJECT
public:
	WebTab(MainWindow* parent = 0);
	~WebTab();
	
	virtual void activate();
	
	bool beginSetup();
	virtual void completeSetup();
	
	bool close();
	
	//! Loads an unformatted URL
	void load(QString url, bool hideUrl = false);
	QString current();
	
public slots:
	void copy();
	void cut();
	void paste();
	
	void go();
	void refresh();
	
	//! Opens current web page in system's default browser
	void openInBrowser();
	
private slots:
	void updateTitle(const QString& title);
	void updateUrl(const QUrl& url);
	
	void on_actionGo_triggered();
	
	void on_ui_prevFind_clicked();
	void on_ui_nextFind_clicked();
	
	//! Check if there was an error loading the page
	void on_ui_webView_loadFinished(bool ok);
	
	void refreshSettings();

protected:
	QWebView* webView();
	
private slots:
	void linkClicked(const QUrl& url);

private:
	QUrl m_prevUrl;
	MacroString m_fragmentMacro;
	AudioTutorial* m_audioTutorial;
};

#endif