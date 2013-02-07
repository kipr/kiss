#include "kiss_standard_environment.hpp"

#include "log.hpp"
#include "target.hpp"
#include "interface_manager.hpp"
#include "interface_plugin_manager.hpp"
#include "language_helper_plugin_manager.hpp"
#include "lexer_plugin_manager.hpp"

#include <QIcon>
#include <QApplication>
#include <QMetaType>
#include <QDir>
#include <QDebug>

#include <cstdio>

using namespace Kiss;

void debugLogHandler(QtMsgType type, const char *msg)
{
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
		fprintf(stderr, "FATAL ERROR: \"%s\"\n", msg);
		fprintf(stderr, "KISS IDE is unexpectedly going down. Sorry about that. :(\n");
		abort();
	}
}

void StandardEnvironment::createStandardEnvironment()
{
	// Install debug handler
	qInstallMsgHandler(debugLogHandler);
	
	// Setup QApplication
#ifdef Q_OS_MAC
	QDir::setCurrent(QApplication::applicationDirPath() + "/../");
#else
	QDir::setCurrent(QApplication::applicationDirPath());
#endif
	
	qRegisterMetaType<Target::TargetPtr>("TargetPtr");
	
	QApplication::setOrganizationName("KIPR");
	QApplication::setOrganizationDomain("kipr.org");
	QApplication::setApplicationName("KISS");
	QApplication::setWindowIcon(QIcon(":/icon.png"));
	
	LanguageHelperPluginManager::ref().loadAll();
	Target::InterfacePluginManager::ref().loadAll();
	Lexer::PluginManager::ref().loadAll();
}
