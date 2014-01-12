#include "kiss_standard_environment.hpp"

#include "log.hpp"
#include "target.hpp"
#include "interface_manager.hpp"
#include "lexer_factory.hpp"
#include "language_helper_manager.hpp"

#include "kovan_interface.hpp"
#include "kovan_serial_interface.hpp"
#include "my_computer_interface.hpp"
#include "my_ks2_interface.hpp"

#include "lexer_c.hpp"
#include "lexer_cpp.hpp"
#include "lexer_java.hpp"
#include "lexer_javascript.hpp"
#include "lexer_python.hpp"

#include "language_helper_c.hpp"

#include <QIcon>
#include <QApplication>
#include <QMetaType>
#include <QDir>
#include <QDebug>
#include <QTranslator>

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
  
  QTranslator *translator = new QTranslator(QApplication::instance());
  translator->load("KISS_zh", QDir::current().filePath("locale"));
  QApplication::instance()->installTranslator(translator);
	
	LanguageHelperManager::ref().addLanguageHelper(new LanguageHelperC);
  
  using namespace target;
  InterfaceManager::ref().addInterface(new KovanInterface);
  InterfaceManager::ref().addInterface(new KovanSerialInterface);
  InterfaceManager::ref().addInterface(new MyKs2Interface);
  InterfaceManager::ref().addInterface(new MyComputerInterface);
  
  using namespace lexer;
  Factory::ref().registerLexerConstructor(new ConstructorC);
  Factory::ref().registerLexerConstructor(new ConstructorCPP);
  Factory::ref().registerLexerConstructor(new ConstructorJava);
  Factory::ref().registerLexerConstructor(new ConstructorJavaScript);
  Factory::ref().registerLexerConstructor(new ConstructorPython);
  
  // lexer::PluginManager::ref().loadAll();
}
