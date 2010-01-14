/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
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

#include <QApplication>
#include <QDir>
#include <QSplashScreen>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	/* The Following lines just set up the application object */
	QApplication application(argc, argv);
	

#ifdef Q_OS_MAC
	QDir::setCurrent(QApplication::applicationDirPath() + "../../../");
#else
	QDir::setCurrent(QApplication::applicationDirPath());	
#endif

	QApplication::setOrganizationName("KIPR");
	QApplication::setOrganizationDomain("kipr.org");
	QApplication::setApplicationName("KISS");
	QApplication::setWindowIcon(QIcon(":/icon.png"));
	/* Now actually do stuff */
	
	QPixmap splashPixmap(":/splash_screen.png");
	QSplashScreen splash(splashPixmap);
	MainWindow window;
	splash.show();
	QTimer::singleShot(1500, &splash, SLOT(hide()));
	QTimer::singleShot(1000, &window, SLOT(show()));

	if(argc < 2 || !window.openFile(QApplication::arguments()[1]))
		QTimer::singleShot(1500, &window, SLOT(on_actionChange_Target_triggered()));

	return application.exec();
}
