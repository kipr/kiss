#include "kiss_standard_environment.hpp"

#include "log.hpp"
#include "target.hpp"
#include "interface_manager.hpp"
#include "interface_plugin_manager.hpp"
#include "language_helper_plugin_manager.hpp"

#include <QIcon>
#include <QApplication>
#include <QMetaType>
#include <QDir>
#include <QDebug>

#include <cstdio>

using namespace kiss;

void debugLogHandler(QtMsgType type, const QMessageLogContext &m, const QString &msg)
{
	switch (type) {
	case QtDebugMsg:
		Log::ref().debug(msg.toUtf8());
		break;
	case QtWarningMsg:
		Log::ref().warning(msg.toUtf8());
		break;
	case QtCriticalMsg:
		Log::ref().error(msg.toUtf8());
		break;
	case QtFatalMsg:
		Log::ref().error(msg.toUtf8());
		fprintf(stderr, "FATAL ERROR: \"%s\"\n", msg.toUtf8().data());
		fprintf(stderr, "KISS IDE is unexpectedly going down. Sorry about that. :(\n");
		abort();
	}
}

void StandardEnvironment::createStandardEnvironment()
{
	// Install debug handler
	qInstallMessageHandler(debugLogHandler);
	
	// Setup QApplication
#ifdef Q_OS_MAC
	QDir::setCurrent(QApplication::applicationDirPath() + "/../");
#else
	QDir::setCurrent(QApplication::applicationDirPath());
#endif
	
	qRegisterMetaType<target::TargetPtr>("TargetPtr");
	
	QApplication::setOrganizationName("KIPR");
	QApplication::setOrganizationDomain("kipr.org");
	QApplication::setApplicationName("KISS");
	QApplication::setWindowIcon(QIcon(":/icon.png"));
	
	LanguageHelperPluginManager::ref().loadAll();
	target::InterfacePluginManager::ref().loadAll();
}
