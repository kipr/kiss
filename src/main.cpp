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
#include "DeclarativeTab.h"
#include "KissArchive.h"
#include "TargetMenu.h"
#include "LogStreamBuf.h"
#include "AudioTutorial.h"
#include "Log.h"
#include "LogWindow.h"

#include "Compiler.h"
#include "TestCompilerC.h"
#include "TestCompilerO.h"

#include <QTimer>
#include <QDebug>
#include <BackendCapabilities>

using namespace std;

void createArchive(const QString& name, const unsigned version, const QString& platforms, const QString& fileList, const QString& out) {
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
	KissReturn ret = KissArchive::create(name, version, platforms.split(','),
		QString(f.readAll().data()).split('\n'), 
		&outf);
		
	if(ret.error) qWarning() << "Archive creation failed!" << ret.message;
}

void handleArgs() 
{
	const QStringList& args = QApplication::arguments();
	if(args[1] == "--createArchive") {
		if(args.size() != 7) {
			qWarning() << "Wrong number of arguments";
			return;
		}
		createArchive(args[2], args[3].toUInt(), args[4], args[5], args[6]);
	} if(args[1] == "--uninstall") {
		if(args.size() != 3) {
			qWarning() << "Wrong number of arguments";
			return;
		}
		KissArchive::uninstall(args[2]);
	} else if(args[1] == "--install") {
		foreach(const QString arg, args.mid(2)) {
			QFile f(arg);
			if(!f.open(QIODevice::ReadOnly)) {
				qWarning() << "Unable to open" << arg << "for reading";
				continue;
			}
	
			KissArchive::install(&f);
		}
	} else if(args[1] == "--list") {
		foreach(const QString arg, args.mid(2)) {
			qWarning() << arg << ":";
			QFile f(arg);
			if(!f.open(QIODevice::ReadOnly)) {
				qWarning() << "Unable to open" << arg << "for reading";
				continue;
			}

			foreach(const QString& file, KissArchive::list(&f)) {
				qWarning() << "\t" << file;
			}
		}
	}
}

void debugLogHandler(QtMsgType type, const char *msg)
{
	//in this function, you can write the message to any stream!
	switch (type) {
	case QtDebugMsg:
		Log::ref().debug(msg);
		break;
	case QtWarningMsg:
		Log::ref().warning(msg);
		break;
	case QtCriticalMsg:
		Log::ref().error(msg);
		break;
	case QtFatalMsg:
		Log::ref().error(msg);
		abort();
	}
}

int main(int argc, char **argv)
{
	QApplication application(argc, argv);
	
#ifdef ENABLE_LOG_WINDOW
	streambuf* restore = cout.rdbuf();
	LogStreamBuf logRedir;
	cout.rdbuf(&logRedir);
	cerr.rdbuf(&logRedir);
	clog.rdbuf(&logRedir);
#endif
	
	Log::ref().info(QString("KISS is starting up... (Qt version: %1)").arg(qVersion()));
	qInstallMsgHandler(debugLogHandler);
	
	if(QApplication::arguments().size() > 1 && QApplication::arguments()[1].startsWith("--")) {
		handleArgs();
		return 0;
	}
	
	#ifdef Q_OS_MAC
		QDir::setCurrent(QApplication::applicationDirPath() + "/../");
	#else
		QDir::setCurrent(QApplication::applicationDirPath());
	#endif

	QApplication::setOrganizationName("KIPR");
	QApplication::setOrganizationDomain("kipr.org");
	QApplication::setApplicationName("KISS");
	QApplication::setWindowIcon(QIcon(":/icon.png"));
	
	CompilerManager::ref().addCompiler(new TestCompilerC());
	CompilerManager::ref().addCompiler(new TestCompilerO());
	
	QPixmap splashPixmap(":/splash_screen.png");
	QSplashScreen splash(splashPixmap);
	splash.raise();
	splash.show();
	MainWindow mainWindow;
#ifdef BUILD_DECLARATIVE_TAB
	mainWindow.addTab(new DeclarativeTab(QUrl("qrc:/welcome/welcome.qml"), &mainWindow));
#else
#ifdef BUILD_WEB_TAB
	mainWindow.addTab(new WelcomeTab(&mainWindow));
#endif
#endif
	Log::ref().info(QString("Starting with the following arguments: [%1]").arg(QApplication::arguments().join(", ")));
	foreach(const QString& arg, QApplication::arguments().mid(1)) mainWindow.openFile(arg);
	QTimer::singleShot(1500, &splash, SLOT(hide()));
	QTimer::singleShot(1000, &mainWindow, SLOT(show()));
	QTimer::singleShot(1000, &mainWindow, SLOT(raise()));

#ifdef ENABLE_LOG_WINDOW
#ifdef BUILD_DEVELOPER_TOOLS
	Log::ref().info("Built with developer tools. Automatically showing error log.");
	QTimer::singleShot(1500, &LogWindow::ref(), SLOT(show()));
#endif
#endif

	Log::ref().info("KISS has been launched");
	int ret = application.exec();
	
#ifdef ENABLE_LOG_WINDOW
	cout.rdbuf(restore);
	cerr.rdbuf(restore);
	clog.rdbuf(restore);
#endif
	
	return ret;
}
