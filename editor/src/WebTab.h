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

class MainWindow;

class WebTab : public QWidget, public Tab, private Ui::WebTab
{
	Q_OBJECT
public:
	WebTab(MainWindow* mainWindow);
	~WebTab();
	
	void addActionsFile(QMenu* file);
	void addActionsEdit(QMenu* edit);
	void addActionsHelp(QMenu* help);
	void addOtherActions(QMenuBar* menuBar);
	void addToolbarActions(QToolBar* toolbar);
	
	bool beginSetup();
	virtual void completeSetup();
	
	bool close();
	
	void load(QString url, bool hideUrl = false);
	
	void moveTo(int line, int pos);
	
private slots:
	void updateTitle(QString title);
	void updateUrl(QUrl url);
	
	void on_actionGo_triggered();
	void on_actionCopy_triggered();
	void on_actionCut_triggered();
	void on_actionPaste_triggered();
	
	void refreshSettings();

protected:
	QWebView* webView();	

private:
};

#endif