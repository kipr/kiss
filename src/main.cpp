/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
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

#include "main_window.hpp"
#include "welcome_tab.hpp"
#include "declarative_tab.hpp"
#include "log_stream_buf.hpp"
#include "log.hpp"
#include "log_window.hpp"
#include "kiss_standard_environment.hpp"

#include <QTimer>
#include <QDebug>
#include <BackendCapabilities>
#include <QApplication>
#include <QDir>
#include <iostream>
#include <QSplashScreen>

using namespace std;
using namespace Kiss;

int main(int argc, char **argv)
{
	QApplication application(argc, argv);
	
#ifdef ENABLE_LOG_WINDOW
	streambuf *restore = cout.rdbuf();
	LogStreamBuf logRedir;
	cout.rdbuf(&logRedir);
	cerr.rdbuf(&logRedir);
	clog.rdbuf(&logRedir);
#endif
	
	Log::ref().info(QString("KISS IDE is starting up... (Qt version: %1)").arg(qVersion()));
	
	// Creates everything we need
	StandardEnvironment::createStandardEnvironment();
	
	Widget::MainWindow mainWindow;
	
#ifdef BUILD_DECLARATIVE_TAB
	mainWindow.addTab(new Widget::DeclarativeTab(QUrl("qrc:/welcome/welcome.qml"), &mainWindow));
#else
#ifdef BUILD_WEB_TAB
	mainWindow.addTab(new Widget::WelcomeTab(&mainWindow));
#endif
#endif

	Log::ref().info(QString("Starting with the following arguments: [%1]").arg(QApplication::arguments().join(", ")));
	foreach(const QString& arg, QApplication::arguments().mid(1)) {
		mainWindow.openFile(arg);
	}
	
	mainWindow.show();
	mainWindow.raise();

#ifdef ENABLE_LOG_WINDOW
#ifdef BUILD_DEVELOPER_TOOLS
	Log::ref().info("Built with developer tools. Automatically showing error log.");
	QTimer::singleShot(500, &Widget::LogWindow::ref(), SLOT(show()));
#endif
#endif

	Log::ref().info("KISS IDE... hassha!");
	int ret = application.exec();
	
#ifdef ENABLE_LOG_WINDOW
	cout.rdbuf(restore);
	cerr.rdbuf(restore);
	clog.rdbuf(restore);
#endif
	
	return ret;
}
