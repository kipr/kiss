#include "kiss_standard_environment.hpp"

#include "log.hpp"
#include "target.hpp"
#include "interface_manager.hpp"
#include "interface_plugin_manager.hpp"
#include "lexer_plugin_manager.hpp"

#include <QIcon>
#include <QApplication>
#include <BackendCapabilities>
#include <QDir>
#include <QDebug>

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
	
	Target::InterfacePluginManager::ref().loadAll();
	Lexer::PluginManager::ref().loadAll();
}
