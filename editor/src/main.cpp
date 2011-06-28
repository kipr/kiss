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
#include "WelcomeTab.h"
#include "KissArchive.h"
#include <QTimer>
#include <QDebug>

void createArchive(QString name, unsigned version, QString fileList, QString out) {
	QFile f(fileList);
	if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Unable to open" << fileList << "for reading.";
		return;
	}
	QFile outf(out);
	if(!outf.open(QIODevice::WriteOnly)) {
		qWarning() << "Unable to open" << out << "for writing.";
		return;
	}
	bool ret = KissArchive::create(name, version, 
		QString(f.readAll().data()).split('\n'), 
		&outf);
		
	if(!ret) qWarning() << "Archive creation failed!";
}

int main(int argc, char **argv)
{
	/* The Following lines just set up the application object */
	QApplication application(argc, argv);
	
	const QStringList& args = QApplication::arguments();
	if(args.size() == 6) {
		if(args[1] == "--createArchive") {
			createArchive(args[2], args[3].toUInt(), args[4], args[5]);
		}
		
		return 0;
	} else if(args.size() == 3) {
		if(args[1] == "--uninstall") {
			KissArchive::uninstall(args[2]);
			return 0;
		}
		
		QFile f(args[2]);
		if(!f.open(QIODevice::ReadOnly)) {
			return 0;
		}
		
		KissArchive::install(&f);
		
		return 0;
	}
	

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
	splash.show();
	MainWindow::ref().addTab(new WelcomeTab(&MainWindow::ref()));
	QTimer::singleShot(1500, &splash, SLOT(hide()));
	QTimer::singleShot(1000, &MainWindow::ref(), SLOT(show()));

	return application.exec();
}
