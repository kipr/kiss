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

WelcomeTab::WelcomeTab(QWidget* parent) : WebTab(parent) {}


void WelcomeTab::addActionsEdit(QMenu*) {}

void WelcomeTab::addOtherActions(QMenuBar*) {}

void WelcomeTab::addToolbarActions(QToolBar*) {}

void WelcomeTab::activate()
{
	WebTab::activate();
	MainWindow::ref().hideErrors();
}

void WelcomeTab::completeSetup()
{	
	WebTab::completeSetup(); 
	load("", true); // Hide URL Bar
	
	QFile file(":/welcome/index.html");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QByteArray data = file.readAll();
#ifndef Q_OS_WIN
	data.replace("KISS_BACKGROUND", palette().color(QPalette::Background).name().toAscii());
#else
	data.replace("KISS_BACKGROUND", "#FFFFFF");
#endif
	
	webView()->setHtml(data.data());
	
	actionOpenInBrowser->setEnabled(false);
}