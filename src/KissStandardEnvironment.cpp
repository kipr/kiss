#include "KissStandardEnvironment.h"

#include "Log.h"

#include "Device.h"
#include "InterfaceManager.h"

#include <kiss-compiler/CompilerPluginManager.h>
#include "InterfacePluginManager.h"
#include "LexerPluginManager.h"

#include <QIcon>
#include <QApplication>
#include <BackendCapabilities>
#include <QDir>
#include <QDebug>

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

void KissStandardEnvironment::createStandardEnvironment()
{
	// Install debug handler
	qInstallMsgHandler(debugLogHandler);
	
	// Setup QApplication
	#ifdef Q_OS_MAC
		QDir::setCurrent(QApplication::applicationDirPath() + "/../");
	#else
		QDir::setCurrent(QApplication::applicationDirPath());
	#endif
	
	qRegisterMetaType<DevicePtr>("DevicePtr");
	
	QApplication::setOrganizationName("KIPR");
	QApplication::setOrganizationDomain("kipr.org");
	QApplication::setApplicationName("KISS");
	QApplication::setWindowIcon(QIcon(":/icon.png"));
	
	CompilerPluginManager::ref().loadAll();
	InterfacePluginManager::ref().loadAll();
	LexerPluginManager::ref().loadAll();
}